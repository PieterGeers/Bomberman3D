#include "stdafx.h"
#include "BomberManScene.h"

#include "PhysxProxy.h"
#include "FixedCamera.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"
#include "PhysxManager.h"
#include "RigidBodyComponent.h"
#include "ModelComponent.h"
#include "ModelAnimator.h"
#include "ContentManager.h"
#include "../CharacterController.h"
#include "../../../Materials/Shadow/SkinnedDiffuseMaterial_Shadow.h"
#include "../../../Materials/DiffuseMaterial.h"
#include "../TilePrefab.h"
#include "../Bom.h"
#include "../../../Materials/Shadow/DiffuseMaterial_Shadow.h"
#include "../TimerPrefab.h"
#include "../ParticlePrefab.h"
#include "SceneManager.h"
#include "../PowerUpUI.h"
#include "SpriteComponent.h"
#include "BomberManGameOver.h"
#include "SoundManager.h"
#include "../PostProcessing/PostColorBalance.h"

BomberManScene::BomberManScene()
	:GameScene(L"BomberManScene")
{
}

BomberManScene::~BomberManScene()
{
	for (auto b : Bom::GetBombsToRemove())
	{
		SafeDelete(b);
	}
	if (!m_PostProcessingActive)
		delete m_pColorBalance;
}

void BomberManScene::UnfreezePlayers()
{
	for (unsigned i = 0; i != m_pPlayer.size(); ++i)
	{
		static_cast<CharacterController*>(m_pPlayer[i])->UnFreeze();
	}
}

void BomberManScene::Restart()
{
	m_Pause = false;
	if (!m_StartGame && m_CurrentRound < 3)
		RemoveChild(m_pCountDown, true);
	m_CurrentRound = 0;
	SceneDeactivated();
	m_CurrentRound = 0;
	m_pUI[0]->Restart();
	m_pUI[1]->Restart();
	m_TutorialTime = 5.0f;
	m_HasTutorialPlayed = false;
	m_pTutorial = new GameObject;
	auto tut = new SpriteComponent(L"./Resources/Textures/BomberMan/Controller.png");
	m_pTutorial->AddComponent(tut);
	AddChild(m_pTutorial);
	m_pCountDown->Freeze();
}

void BomberManScene::Initialize()
{
	const auto gameContext = GetGameContext();
	GetPhysxProxy()->EnablePhysxDebugRendering(m_EnableDebugRenderer);
	gameContext.pShadowMapper->SetLight({ -75.f,75.f,75.f }, { 0.305109f, -0.915801f, -0.261185f });
	auto physX = PhysxManager::GetInstance()->GetPhysics();

	//INPUT
	//*****
	gameContext.pInput->AddInputAction(InputAction(Bomb_P1, Pressed, -1, -1, XINPUT_GAMEPAD_A, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(RemoteBom_P1, Pressed, -1, -1, XINPUT_GAMEPAD_X, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(Up_P1, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_UP, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(Down_P1, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(Left_P1, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(Right_P1, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(Bomb_P2, Pressed, -1, -1, XINPUT_GAMEPAD_A, PlayerTwo));
	gameContext.pInput->AddInputAction(InputAction(RemoteBom_P2, Pressed, -1, -1, XINPUT_GAMEPAD_X, PlayerTwo));
	gameContext.pInput->AddInputAction(InputAction(Up_P2, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_UP, PlayerTwo));
	gameContext.pInput->AddInputAction(InputAction(Down_P2, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_DOWN, PlayerTwo));
	gameContext.pInput->AddInputAction(InputAction(Left_P2, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_LEFT, PlayerTwo));
	gameContext.pInput->AddInputAction(InputAction(Right_P2, Pressed, -1, -1, XINPUT_GAMEPAD_DPAD_RIGHT, PlayerTwo));
	gameContext.pInput->AddInputAction(InputAction(ShowDebugRenderer, Pressed, 'R'));
	gameContext.pInput->AddInputAction(InputAction(SetShadowPosition, Pressed, VK_SPACE));
	gameContext.pInput->AddInputAction(InputAction(ShowDebugCamera, Pressed, 'C'));
	gameContext.pInput->AddInputAction(InputAction(ShowPostProcessing, Pressed, 'P'));

	//CAMERA
	//******
	m_pDebugCamera = gameContext.pCamera;
	auto FixedCam = new FixedCamera();
	AddChild(FixedCam);
	m_pNewCamera = FixedCam->GetComponent<CameraComponent>();
	m_pNewCamera->GetTransform()->Translate(0, 700, -325);
	m_pNewCamera->GetTransform()->Rotate(115, 180, 0);
	m_pNewCamera->SetFieldOfView(50.f);
	SetActiveCamera(m_pNewCamera);

	//GROUND PLANE
	//************
	auto pMaterial = physX->createMaterial(0, 0, 0);

	auto pGround = new GameObject();
	pGround->AddComponent(new RigidBodyComponent(true));
	std::shared_ptr<physx::PxGeometry> geom(new physx::PxPlaneGeometry());
	pGround->AddComponent(new ColliderComponent(geom, *pMaterial, physx::PxTransform(physx::PxQuat(DirectX::XM_PIDIV2, physx::PxVec3(0, 0, 1)))));
	AddChild(pGround);

	//GROUND MESH
	//***********
	auto diffMat2 = new DiffuseMaterial_Shadow();
	diffMat2->SetDiffuseTexture(L"./Resources/Textures/BomberMan/GroundPlane.png");
	gameContext.pMaterialManager->AddMaterial(diffMat2, 6);

	auto pGroundObj = new GameObject();
	auto pGroundModel = new ModelComponent(L"./Resources/Meshes/BomberMan/UnitPlane.ovm");
	pGroundModel->SetMaterial(6);
	pGroundObj->AddComponent(pGroundModel);
	pGroundObj->GetTransform()->Translate(0, -15, 0);
	pGroundObj->GetTransform()->Scale(40.0f, 1, 30.0f);
	AddChild(pGroundObj);

	//TOP PART LEVEL
	//**************
	auto TLDiffuseMaterial = new DiffuseMaterial_Shadow();
	TLDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/BomberMan/TopLevel_Diffuse.png");
	TLDiffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(TLDiffuseMaterial, 1);

	auto pLevel1 = new GameObject();
	auto topLevelModel = new ModelComponent(L"Resources/Meshes/BomberMan/BomberManTopLevel.ovm");
	topLevelModel->SetMaterial(1);
	RigidBodyComponent* levelRigidbody = new RigidBodyComponent(true);
	std::shared_ptr<physx::PxGeometry> geometry = std::make_shared<physx::PxTriangleMeshGeometry>(ContentManager::Load<physx::PxTriangleMesh>(L"Resources/Meshes/BomberMan/BomberManTopLevel.ovpt"));
	ColliderComponent * collider = new ColliderComponent(geometry, *pMaterial);
	pLevel1->GetTransform()->Translate(0, 0, 0);
	pLevel1->GetTransform()->Rotate(90, 0, 0);
	pLevel1->AddComponent(topLevelModel);
	pLevel1->AddComponent(levelRigidbody);
	pLevel1->AddComponent(collider);
	AddChild(pLevel1);

	//LOWER PART LEVEL
	//****************
	auto LLDiffuseMaterial = new DiffuseMaterial_Shadow();
	LLDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/BomberMan/LowerLevel_Diffuse.png");
	LLDiffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(LLDiffuseMaterial, 2);

	auto pLevel2 = new GameObject();
	auto lowerLevelModel = new ModelComponent(L"Resources/Meshes/BomberMan/BomberManLowerLevel.ovm");
	lowerLevelModel->SetMaterial(2);
	pLevel2->GetTransform()->Scale(0.15f, 0.15f, 0.15f);
	pLevel2->GetTransform()->Translate(0, 0, 0);
	pLevel2->GetTransform()->Rotate(90, 0, 0);
	pLevel2->AddComponent(lowerLevelModel);
	AddChild(pLevel2);

	//CHARACTER 1
	//***********
	auto m_pPlayer1 = new CharacterController(5,20,GamepadIndex::PlayerOne);
	AddChild(m_pPlayer1);

	auto skinnedDiffuseMaterial = new SkinnedDiffuseMaterial_Shadow();
	skinnedDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/BomberMan/Boy01_diffuse.jpg");
	skinnedDiffuseMaterial->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial, 0);

	auto model = new GameObject();
	auto modelComp = new ModelComponent(L"./Resources/Meshes/BomberMan/BombermanCharacter.ovm");
	modelComp->SetMaterial(0);
	model->AddComponent(modelComp);
	m_pPlayer1->AddChild(model);
	modelComp->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	model->GetTransform()->Translate(0, -10, 0);
	m_pPlayer1->GetTransform()->Translate(-75, 10, 75);
	m_pPlayer1->GetComponent<ModelComponent>(true)->GetAnimator()->Play();
	m_pPlayer1->SetTag(L"Player1");
	m_pPlayer.push_back(m_pPlayer1);

	//CHARACTER 2
	//***********
	auto m_pPlayer2 = new CharacterController(5, 20, GamepadIndex::PlayerTwo);
	AddChild(m_pPlayer2);

	auto skinnedDiffuseMaterial2 = new SkinnedDiffuseMaterial_Shadow();
	skinnedDiffuseMaterial2->SetDiffuseTexture(L"./Resources/Textures/BomberMan/Boy02_diffuse.png");
	skinnedDiffuseMaterial2->SetLightDirection(gameContext.pShadowMapper->GetLightDirection());
	gameContext.pMaterialManager->AddMaterial(skinnedDiffuseMaterial2, 3);

	auto model2 = new GameObject();
	auto modelComp2 = new ModelComponent(L"./Resources/Meshes/BomberMan/BombermanCharacter.ovm");
	modelComp2->SetMaterial(3);
	model2->AddComponent(modelComp2);
	m_pPlayer2->AddChild(model2);
	modelComp2->GetTransform()->Scale(0.1f, 0.1f, 0.1f);
	model2->GetTransform()->Translate(0, -10, 0);
	m_pPlayer2->GetTransform()->Translate(75, 0, -75);
	m_pPlayer2->GetComponent<ModelComponent>(true)->GetAnimator()->Play();
	m_pPlayer2->SetTag(L"Player2");
	m_pPlayer.push_back(m_pPlayer2);

	//FREEZE PLAYERS AT START
	//***********************
	for (unsigned i = 0; i != m_pPlayer.size(); ++i)
	{
		static_cast<CharacterController*>(m_pPlayer[i])->Freeze();
	}

	//BOMB
	//****
	auto BomDiffuseMaterial = new DiffuseMaterial();
	BomDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/BomberMan/Bomb_Diffuse.png");
	gameContext.pMaterialManager->AddMaterial(BomDiffuseMaterial, 4);
	
	//REMOTE BOMB
	//***********
	auto remoteDiffuse = new DiffuseMaterial();
	remoteDiffuse->SetDiffuseTexture(L"./Resources/Textures/BomberMan/Remote_Diffuse.png");
	gameContext.pMaterialManager->AddMaterial(remoteDiffuse, 10);
	
	//TILE
	//****
	auto TileDiffuseMaterial = new DiffuseMaterial();
	TileDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/BomberMan/Destroyable_Diffuse.jpg");
	gameContext.pMaterialManager->AddMaterial(TileDiffuseMaterial, 5);

	//POWER UP
	//********
	auto PowerUpDiffuseMaterial = new DiffuseMaterial();
	PowerUpDiffuseMaterial->SetDiffuseTexture(L"./Resources/Textures/BomberMan/PowerUpTexture.png");
	gameContext.pMaterialManager->AddMaterial(PowerUpDiffuseMaterial, 7);

	//LEVEL GRID
	//**********
	float x = -75.f, y = 75.f;
	for (int i = 0; i < 11; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			m_LevelGrid[j][i].Position = { x,y };
			if (i % 2 == 1)
			{
				if (j % 2 == 1)
				{
					m_LevelGrid[j][i].IsPartOfLevel = true;
					m_LevelGrid[j][i].hasStone = false;
				}
			}
			x += 15.0f;
		}
		y -= 15.0f;
		x = -75.0f;
	}
	m_LevelGrid[0][0].hasStone = false;
	m_LevelGrid[1][0].hasStone = false;
	m_LevelGrid[0][1].hasStone = false;
	m_LevelGrid[10][10].hasStone = false;
	m_LevelGrid[9][10].hasStone = false;
	m_LevelGrid[10][9].hasStone = false;
	for (int i = 0; i < 11; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			if (i == 0 && (j == 0 || j == 1))
				continue;
			if (i == 1 && j == 0)
				continue;
			if (i == 9 && j == 10)
				continue;
			if (i == 10 && (j == 9 || j == 10))
				continue;
			if (m_LevelGrid[i][j].hasStone)
			{
				auto t = new TilePrefab(m_LevelGrid[i][j].Position);
				AddChild(t);
				m_LevelGrid[i][j].TilePrefab = t;
			}
		}
	}

	//TIME
	//****
	m_pTimer = new TimerPrefab(DirectX::XMFLOAT3(640, 0, 0.95f), 180.0f);
	m_pTimer->SetPaused(true);
	AddChild(m_pTimer);

	//COUNTDOWN
	//*********
	m_pCountDown = new CountDownPrefab();
	AddChild(m_pCountDown);
	m_pCountDown->Freeze();

	//POWER UP UI
	//***********
	auto ui1 = new PowerUpUI({ 128.f,360.f,.98f }, {-175.0f, -500.f,.98f} ,L"Player1", true);
	AddChild(ui1);
	ui1->SetTag(L"Player1UI");
	ui1->Freeze();
	auto ui2 = new PowerUpUI({ 1152.f,360.f,.98f }, {175.f, -500.f, .98f}, L"Player2", false);
	AddChild(ui2);
	ui2->SetTag(L"Player2UI");
	ui2->Freeze();
	m_pUI.push_back(ui1);
	m_pUI.push_back(ui2);

	//TUTORIAL
	//********
	m_pTutorial = new GameObject;
	auto tut = new SpriteComponent(L"./Resources/Textures/BomberMan/Controller.png");
	m_pTutorial->AddComponent(tut);
	AddChild(m_pTutorial);

	//POST PROCESSING EFFECTS
	//***********************
	m_pColorBalance = new PostColorBalance(0.1f, 0.0f, -.1f);
	AddPostProcessingEffect(m_pColorBalance);
	
}

void BomberManScene::CheckIfDead(const int x, const int y, int bomRange)
{
	if (HasExploded(m_pPlayer[0], x, y, bomRange))
	{
		if (HasExploded(m_pPlayer[1], x, y, bomRange))
		{
			//std::cout << "Both players died" << std::endl;
			m_pUI[0]->AddPoint();
			m_pUI[1]->AddPoint();
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
		}
		else
		{
			m_pUI[1]->AddPoint();
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
		}
	}
	else if (HasExploded(m_pPlayer[1], x,y,bomRange))
	{
		if (HasExploded(m_pPlayer[0], x, y, bomRange))
		{
			//std::cout << "Both players died" << std::endl;
			m_pUI[0]->AddPoint();
			m_pUI[1]->AddPoint();
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
		}
		else
		{
			m_pUI[0]->AddPoint();
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
		}
	}
}

void BomberManScene::Update()
{
	const auto gameContext = GetGameContext();
	
	//GAMEOVER
	GameOver();

	//PAUSE
	//*****
	if (gameContext.pInput->IsActionTriggered(Pause_P1))
	{
		m_Pause = true;
		SceneManager::GetInstance()->SetActiveGameScene(L"BomberManInGamePauseMenu");
	}

	//TUTORIAL
	//********
	if (!m_HasTutorialPlayed)
	{
		m_TutorialTime -= GetGameContext().pGameTime->GetElapsed();
		if (m_TutorialTime <= 0.0f)
		{
			m_HasTutorialPlayed = true;
			RemoveChild(m_pTutorial, true);
			m_pCountDown->UnFreeze();
			m_pUI[0]->UnFreeze();
			m_pUI[1]->UnFreeze();
		}
		else
			return;
	}

	//COUNT DOWN
	//**********
	if (!m_StartGame)
	{
		if (m_pCountDown != nullptr && !m_pCountDown->GetCountDownComplete())
			return;
		m_StartGame = true;
		if (m_pTimer->IsPaused())
			m_pTimer->SetPaused(false);
	}
	//PLAYERS
	//*******
	for (unsigned i = 0; i != m_pPlayer.size(); ++i )
	{
		if (static_cast<CharacterController*>(m_pPlayer[i])->UpdatePowerUps())
		{
			m_pUI[i]->ResetSelected();
		}
	}

	//INPUT
	//*****
	if (gameContext.pInput->IsActionTriggered(Bomb_P1))
	{
		if (static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() == remote && m_pPlayer1RemoteBom != nullptr)
		{
			PVector2 temp = GetGridIndex(m_pPlayer1RemoteBom);
			m_pPlayer1RemoteBom->SetBlocksToDestroy(GetDestroy(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false));
			m_pPlayer1RemoteBom = nullptr;
			CheckIfDead(static_cast<const int>(temp.x), static_cast<const int>(temp.y), 2);
			ExplodeBom(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false);
		}
		SpawnBomb(m_pPlayer[0], static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp());
	}
	if (gameContext.pInput->IsActionTriggered(RemoteBom_P1))
	{
		if (m_pPlayer1RemoteBom != nullptr)
		{
			PVector2 temp = GetGridIndex(m_pPlayer1RemoteBom);
			m_pPlayer1RemoteBom->SetBlocksToDestroy(GetDestroy(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false));
			m_pPlayer1RemoteBom = nullptr;
			CheckIfDead(static_cast<const int>(temp.x), static_cast<const int>(temp.y), 2);
			ExplodeBom(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false);
		}
	}
	if (gameContext.pInput->IsActionTriggered(Up_P1) && static_cast<CharacterController*>(m_pPlayer[0])->HasRange() && static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() != range)
	{
		static_cast<CharacterController*>(m_pPlayer[0])->SetActiveBomPowerUp(range);
		m_pUI[0]->RotateSelected(0);
	}
	else if (gameContext.pInput->IsActionTriggered(Up_P1) && static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() == range)
	{
		static_cast<CharacterController*>(m_pPlayer[0])->SetActiveBomPowerUp(none);
		m_pUI[0]->ResetSelected();
	}
	if (gameContext.pInput->IsActionTriggered(Right_P1) && static_cast<CharacterController*>(m_pPlayer[0])->HasPower() && static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() != power)
	{
		static_cast<CharacterController*>(m_pPlayer[0])->SetActiveBomPowerUp(power);
		m_pUI[0]->RotateSelected(90);
	}
	else if (gameContext.pInput->IsActionTriggered(Right_P1) && static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() == power)
	{
		static_cast<CharacterController*>(m_pPlayer[0])->SetActiveBomPowerUp(none);
		m_pUI[0]->ResetSelected();
	}
	if (gameContext.pInput->IsActionTriggered(Down_P1) && static_cast<CharacterController*>(m_pPlayer[0])->HasUltimate() && static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() != ultimate)
	{
		static_cast<CharacterController*>(m_pPlayer[0])->SetActiveBomPowerUp(ultimate);
		m_pUI[0]->RotateSelected(180);
	}
	else if (gameContext.pInput->IsActionTriggered(Down_P1) && static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() == ultimate)
	{
		static_cast<CharacterController*>(m_pPlayer[0])->SetActiveBomPowerUp(none);
		m_pUI[0]->ResetSelected();
	}
	if (gameContext.pInput->IsActionTriggered(Left_P1) && static_cast<CharacterController*>(m_pPlayer[0])->HasRemote() && static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() != remote)
	{
		static_cast<CharacterController*>(m_pPlayer[0])->SetActiveBomPowerUp(remote);
		m_pUI[0]->RotateSelected(270);
	}
	else if (gameContext.pInput->IsActionTriggered(Left_P1) && static_cast<CharacterController*>(m_pPlayer[0])->GetActiveBomPowerUp() == remote)
	{
		static_cast<CharacterController*>(m_pPlayer[0])->SetActiveBomPowerUp(none);
		m_pUI[0]->ResetSelected();
	}
	if (gameContext.pInput->IsActionTriggered(Bomb_P2))
	{
		if (static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() == remote && m_pPlayer2RemoteBom != nullptr)
		{
			PVector2 temp = GetGridIndex(m_pPlayer2RemoteBom);
			m_pPlayer2RemoteBom->SetBlocksToDestroy(GetDestroy(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false));
			m_pPlayer2RemoteBom = nullptr;
			CheckIfDead(static_cast<const int>(temp.x), static_cast<const int>(temp.y), 2);
			ExplodeBom(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false);
		}
		SpawnBomb(m_pPlayer[1], static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp());
	}
	if (gameContext.pInput->IsActionTriggered(RemoteBom_P2))
	{
		if (m_pPlayer2RemoteBom != nullptr)
		{
			PVector2 temp = GetGridIndex(m_pPlayer2RemoteBom);
			m_pPlayer2RemoteBom->SetBlocksToDestroy(GetDestroy(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false));
			m_pPlayer2RemoteBom = nullptr;
			CheckIfDead(static_cast<const int>(temp.x), static_cast<const int>(temp.y), 2);
			ExplodeBom(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false);
		}
	}
	if (gameContext.pInput->IsActionTriggered(Up_P2) && static_cast<CharacterController*>(m_pPlayer[1])->HasRange() && static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() != range)
	{
		static_cast<CharacterController*>(m_pPlayer[1])->SetActiveBomPowerUp(range);
		m_pUI[1]->RotateSelected(0);
	}
	else if (gameContext.pInput->IsActionTriggered(Up_P2) && static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() == range)
	{
		static_cast<CharacterController*>(m_pPlayer[1])->SetActiveBomPowerUp(none);
		m_pUI[1]->ResetSelected();
	}
	if (gameContext.pInput->IsActionTriggered(Right_P2) && static_cast<CharacterController*>(m_pPlayer[1])->HasPower() && static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() != power)
	{
		static_cast<CharacterController*>(m_pPlayer[1])->SetActiveBomPowerUp(power);
		m_pUI[1]->RotateSelected(90);
	}
	else if (gameContext.pInput->IsActionTriggered(Right_P2) && static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() == power)
	{
		static_cast<CharacterController*>(m_pPlayer[1])->SetActiveBomPowerUp(none);
		m_pUI[1]->ResetSelected();
	}
	if (gameContext.pInput->IsActionTriggered(Down_P2) && static_cast<CharacterController*>(m_pPlayer[1])->HasUltimate() && static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() != ultimate)
	{
		static_cast<CharacterController*>(m_pPlayer[1])->SetActiveBomPowerUp(ultimate);
		m_pUI[1]->RotateSelected(180);
	}
	else if (gameContext.pInput->IsActionTriggered(Down_P2) && static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() == ultimate)
	{
		static_cast<CharacterController*>(m_pPlayer[1])->SetActiveBomPowerUp(none);
		m_pUI[1]->ResetSelected();
	}
	if (gameContext.pInput->IsActionTriggered(Left_P2) && static_cast<CharacterController*>(m_pPlayer[1])->HasRemote() && static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() != remote)
	{
		static_cast<CharacterController*>(m_pPlayer[1])->SetActiveBomPowerUp(remote);
		m_pUI[1]->RotateSelected(270);
	}
	else if (gameContext.pInput->IsActionTriggered(Left_P2) && static_cast<CharacterController*>(m_pPlayer[1])->GetActiveBomPowerUp() == remote)
	{
		static_cast<CharacterController*>(m_pPlayer[1])->SetActiveBomPowerUp(none);
		m_pUI[1]->ResetSelected();
	}

	if (gameContext.pInput->IsActionTriggered(ShowDebugRenderer))
	{
		m_EnableDebugRenderer = !m_EnableDebugRenderer;
		GetPhysxProxy()->EnablePhysxDebugRendering(m_EnableDebugRenderer);
	}
	if (gameContext.pInput->IsActionTriggered(SetShadowPosition))
	{
		gameContext.pShadowMapper->SetLight(gameContext.pCamera->GetTransform()->GetPosition(), gameContext.pCamera->GetTransform()->GetForward());
		std::cout << gameContext.pCamera->GetTransform()->GetForward().x << ", " << gameContext.pCamera->GetTransform()->GetForward().y << ", " << gameContext.pCamera->GetTransform()->GetForward().z << std::endl;
		static_cast<DiffuseMaterial_Shadow*>(gameContext.pMaterialManager->GetMaterial(6))->SetLightDirection(gameContext.pCamera->GetTransform()->GetForward());
		static_cast<DiffuseMaterial_Shadow*>(gameContext.pMaterialManager->GetMaterial(1))->SetLightDirection(gameContext.pCamera->GetTransform()->GetForward());
		static_cast<DiffuseMaterial_Shadow*>(gameContext.pMaterialManager->GetMaterial(2))->SetLightDirection(gameContext.pCamera->GetTransform()->GetForward());
	}
	if (gameContext.pInput->IsActionTriggered(ShowDebugCamera))
	{
		m_EnableDebugCamera = !m_EnableDebugCamera;
		if (m_EnableDebugCamera)
			SetActiveCamera(m_pDebugCamera);
		else
			SetActiveCamera(m_pNewCamera);
	}
	if (gameContext.pInput->IsActionTriggered(ShowPostProcessing))
	{
		m_PostProcessingActive = !m_PostProcessingActive;
		if (m_PostProcessingActive)
		{
			AddPostProcessingEffect(m_pColorBalance);
		}
		else
		{
			RemovePostProcessingEffect(m_pColorBalance);
		}
	}
	//BOMB
	//****
	if (!m_pBomb.empty() && (*m_pBomb.begin())->HasBomExploded())
	{
		PVector2 temp = GetGridIndex(*m_pBomb.begin());
		const int x = static_cast<int>(temp.x);
		const int y = static_cast<int>(temp.y);
		const PowerUp bomType = (*m_pBomb.begin())->GetType();
		int bomRange = 0;
		if (bomType == none || bomType == power)
			bomRange = 3;
		if (bomType == range || bomType == ultimate)
			bomRange = 11;
		CheckIfDead(x, y, bomRange);
		switch (bomType)
		{
		case none: 
			(*m_pBomb.begin())->SetBlocksToDestroy(GetDestroy(x, y, 3, false));
			m_pBomb.pop_front();
			ExplodeBom(x, y, 3, false);
			break;
		case range:
			(*m_pBomb.begin())->SetBlocksToDestroy(GetDestroy(x, y, 11, false));
			m_pBomb.pop_front();
			ExplodeBom(x, y, 11, false);
			break;
		case power:
			(*m_pBomb.begin())->SetBlocksToDestroy(GetDestroy(x, y, 3, true));
			m_pBomb.pop_front();
			ExplodeBom(x, y, 3, true);
			break;
		case ultimate: 
			(*m_pBomb.begin())->SetBlocksToDestroy(GetDestroy(x, y, 11, true));
			m_pBomb.pop_front();
			ExplodeBom(x, y, 11, true);
			break;
		case remote: break;
		case live: break;
		case slow: break;
		default: ;
		}
	}
	if (m_pPlayer1RemoteBom != nullptr)
	{
		if (m_pPlayer1RemoteBom->GetIsExploded())
		{
			PVector2 temp = GetGridIndex(m_pPlayer1RemoteBom);
			m_pPlayer1RemoteBom->SetBlocksToDestroy(GetDestroy(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false));
			m_pPlayer1RemoteBom = nullptr;
			CheckIfDead(static_cast<const int>(temp.x), static_cast<const int>(temp.y), 2);
			ExplodeBom(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false);
		}
	}
	if (m_pPlayer2RemoteBom != nullptr)
	{
		if (m_pPlayer2RemoteBom->GetIsExploded())
		{
			PVector2 temp = GetGridIndex(m_pPlayer2RemoteBom);
			m_pPlayer2RemoteBom->SetBlocksToDestroy(GetDestroy(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false));
			m_pPlayer2RemoteBom = nullptr;
			CheckIfDead(static_cast<const int>(temp.x), static_cast<const int>(temp.y), 2);
			ExplodeBom(static_cast<int>(temp.x), static_cast<int>(temp.y), 2, false);
		}
	}

	//PARTICLE
	//********
	if (!m_pParticles.empty())
	{
		for (auto particle : m_pParticles)
		{
			if (!particle->GetIsAlive())
			{
				particle->Destroy();
				m_pParticles.pop_front();
				std::cout << "[PARTICLE DESTROY COMPLETE]"<< std::endl;
			}
		}
	}

	//TIME
	//****
	if (m_pTimer->GetGameOver())
	{
		m_pTimer->ResetGameOver();
		m_pUI[0]->AddPoint();
		m_pUI[1]->AddPoint();
		SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
	}

	//POWER UP
	//********
	for (unsigned i = 0; i != m_pPowerUps.size(); ++i)
	{
		if (m_pPowerUps[i]->GetHasBeenPickedUp())
		{
			RemoveChild(m_pPowerUps[i], true);
			m_pPowerUps.erase(std::remove(m_pPowerUps.begin(), m_pPowerUps.end(), m_pPowerUps[i]), m_pPowerUps.end());
			break;
		}
	}
}

void BomberManScene::Draw()
{
}

void BomberManScene::SceneActivated()
{
	SoundManager::GetInstance()->GetStream("Game")->Resume();

	if (m_Pause)
		m_Pause = false;
	GetGameContext().pInput->AddInputAction(InputAction(Pause_P1, Pressed, -1, -1, XINPUT_GAMEPAD_BACK, PlayerOne));
}

void BomberManScene::SceneDeactivated()
{
	SoundManager::GetInstance()->GetStream("Game")->Pause();

	m_EnableDebugRenderer = false;
	m_EnableDebugCamera = false;
	GetGameContext().pInput->RemoveInputAction(Pause_P1);

	if (m_Pause)
		return;
	
	m_StartGame = false;
	//LEVEL
	//*****
	for (int i = 0; i < 11; ++i)
	{
		for (int j = 0; j < 11; ++j)
		{
			if ((i == 0 && j == 0) ||
				(i == 1 && j == 0) ||
				(i == 0 && j == 1) ||
				(i == 10 && j == 10) ||
				(i == 9 && j == 10) ||
				(i == 10 && j == 9))
				continue;
			if (m_LevelGrid[i][j].IsPartOfLevel)
				continue;
			if (!m_LevelGrid[i][j].hasStone)
			{
				m_LevelGrid[i][j].hasStone = true;
				auto t = new TilePrefab(m_LevelGrid[i][j].Position);
				AddChild(t);
				m_LevelGrid[i][j].TilePrefab = t;
			}
		}
	}
	//TIMER
	//*****
	m_pTimer->SetPaused(true);
	m_pTimer->SetTime(180);
	//BOM
	//***
	for (auto element : m_pBomb)
	{
		RemoveChild(element, true);
		m_pBomb.pop_front();
	}
	if (m_pPlayer1RemoteBom != nullptr)
	{
		RemoveChild(m_pPlayer1RemoteBom, true);
		m_pPlayer1RemoteBom = nullptr;
	}
	if (m_pPlayer2RemoteBom != nullptr)
	{
		RemoveChild(m_pPlayer2RemoteBom, true);
		m_pPlayer2RemoteBom = nullptr;
	}
	//PARTICLES
	//*********
	for (auto element : m_pParticles)
	{
		element->Destroy();
	}
	m_pParticles.clear();
	////PLAYERS
	////*******
	m_pPlayer[0]->GetTransform()->Translate(-75, 10, 75);
	m_pPlayer[0]->GetComponent<ModelComponent>(true)->GetAnimator()->SetAnimation(idle);
	m_pPlayer[1]->GetTransform()->Translate(75, 10, -75);
	m_pPlayer[1]->GetComponent<ModelComponent>(true)->GetAnimator()->SetAnimation(idle);
	//FREEZE PLAYERS AT START
	//***********************
	for (unsigned i = 0; i != m_pPlayer.size(); ++i)
	{
		static_cast<CharacterController*>(m_pPlayer[i])->Freeze();
		static_cast<CharacterController*>(m_pPlayer[i])->ResetPowerUps();
	}
	//COUNT DOWN
	//**********
	++m_CurrentRound;
	if (m_CurrentRound < 3)
	{
		m_pCountDown = new CountDownPrefab();
		AddChild(m_pCountDown);
	}
}

void BomberManScene::SpawnBomb(GameObject* player, PowerUp type)
{
	PVector2 temp = GetGridIndex(player);
	const int x = static_cast<int>(temp.x);
	const int y = static_cast<int>(temp.y);

	switch (type)
	{
	case none: break;
	case range: 
		static_cast<CharacterController*>(player)->SubRange();
		break;
	case power: 
		static_cast<CharacterController*>(player)->SubPower();
		break;
	case ultimate: 
		static_cast<CharacterController*>(player)->SubUltimate();
		break;
	case remote: 
		static_cast<CharacterController*>(player)->SubRemote();
		break;
	case live: break;
	case slow: break;
	default: ;
	}
	if (type == remote && player == m_pPlayer[0])
	{
		m_pPlayer1RemoteBom = new Bom(true);
		m_pPlayer1RemoteBom->SetType(type);
		AddChild(m_pPlayer1RemoteBom);
		m_pPlayer1RemoteBom->GetTransform()->Translate(m_LevelGrid[x][y].Position.x, 0, m_LevelGrid[x][y].Position.y);
	}
	else if (type == remote && player == m_pPlayer[1])
	{
		m_pPlayer2RemoteBom = new Bom(true);
		m_pPlayer2RemoteBom->SetType(type);
		AddChild(m_pPlayer2RemoteBom);
		m_pPlayer2RemoteBom->GetTransform()->Translate(m_LevelGrid[x][y].Position.x, 0, m_LevelGrid[x][y].Position.y);
	}
	else if (type != remote)
	{
		auto b = new Bom();
		b->SetType(type);
		AddChild(b);
		b->GetTransform()->Translate(m_LevelGrid[x][y].Position.x, 0, m_LevelGrid[x][y].Position.y);
		m_pBomb.push_back(b);
	}
}

std::vector<GameObject*> BomberManScene::GetDestroy(int x, int y, int range, bool explodeAll)
{
	UNREFERENCED_PARAMETER(explodeAll);
	std::vector<GameObject*> tempVec{};
	for (int i = x-1; i > (x-range); --i)
	{
		if (i < 0)
			break;
		if (m_LevelGrid[i][y].IsPartOfLevel)
			break;
		if (m_LevelGrid[i][y].hasStone)
		{
			m_LevelGrid[i][y].hasStone = false;
			if (!explodeAll && m_LevelGrid[i][y].TilePrefab == nullptr)
				break;
			tempVec.push_back(m_LevelGrid[i][y].TilePrefab);
			if (!explodeAll)
				break;
		}
	}
	for (int i = x+1; i < (x + range); ++i)
	{
		if (i > 10)
			break;
		if (m_LevelGrid[i][y].IsPartOfLevel)
			break;
		if (m_LevelGrid[i][y].hasStone)
		{
			m_LevelGrid[i][y].hasStone = false;
			if (!explodeAll && m_LevelGrid[i][y].TilePrefab == nullptr)
				break;
			tempVec.push_back(m_LevelGrid[i][y].TilePrefab);
			if (!explodeAll)
				break;
		}
	}
	for (int i = y-1; i > (y - range); --i)
	{
		if (i < 0)
			break;
		if (m_LevelGrid[x][i].IsPartOfLevel)
			break;
		if (m_LevelGrid[x][i].hasStone)
		{
			m_LevelGrid[x][i].hasStone = false;
			if (!explodeAll && m_LevelGrid[x][i].TilePrefab == nullptr)
				break;
			tempVec.push_back(m_LevelGrid[x][i].TilePrefab);
			if (!explodeAll)
				break;
		}
	}
	for (int i = y+1; i < (y + range); ++i)
	{
		if (i > 10)
			break;
		if (m_LevelGrid[x][i].IsPartOfLevel)
			break;
		if (m_LevelGrid[x][i].hasStone)
		{
			m_LevelGrid[x][i].hasStone = false;
			if (!explodeAll && m_LevelGrid[x][i].TilePrefab == nullptr)
				break;
			tempVec.push_back(m_LevelGrid[x][i].TilePrefab);
			if (!explodeAll)
				break;
		}
	}

	return tempVec;
}

void BomberManScene::ExplodeBom(int x, int y, int range, bool explodeAll)
{
	SoundManager::GetInstance()->GetEffect("Explosion")->Play();
	//LEFT
	//****
	ParticlePrefab* leftParticle = nullptr;
	for (int i = 0; i < range; ++i)
	{
		if (explodeAll)
		{
			if (x - (i + 1) < 0 || m_LevelGrid[x - (i + 1)][y].IsPartOfLevel)
			{
				leftParticle = new ParticlePrefab({ -200,0,0 }, 2.5f + i * 15.0f, 1.f);
				break;
			}
		}
		else
		{
			if (x - (i + 1) < 0 || m_LevelGrid[x - (i + 1)][y].hasStone || m_LevelGrid[x - (i + 1)][y].IsPartOfLevel)
			{
				leftParticle = new ParticlePrefab({ -200,0,0 }, 2.5f + i * 15.0f, 1.f);
				break;
			}
		}
	}
	if (leftParticle == nullptr)
		leftParticle = new ParticlePrefab({ -200,0,0 }, (range - 1) * 15.0f, 1.f);
	leftParticle->GetTransform()->Translate(m_LevelGrid[x][y].Position.x, 7.5f, m_LevelGrid[x][y].Position.y);
	AddChild(leftParticle);
	m_pParticles.push_back(leftParticle);
	//RIGHT
	//*****
	ParticlePrefab* rightParticle = nullptr;
	for (int i = 0; i < range; ++i)
	{
		if (explodeAll)
		{
			if (x + (i + 1) > 10 || m_LevelGrid[x + (i + 1)][y].IsPartOfLevel)
			{
				rightParticle = new ParticlePrefab({ 200,0,0 }, 2.5f + i * 15.0f, 1.f);
				break;
			}
		}
		else
		{
			if (x + (i + 1) > 10 || m_LevelGrid[x + (i + 1)][y].hasStone || m_LevelGrid[x + (i + 1)][y].IsPartOfLevel)
			{
				rightParticle = new ParticlePrefab({ 200,0,0 }, 2.5f + i * 15.0f, 1.f);
				break;
			}
		}
	}
	if (rightParticle == nullptr)
		rightParticle = new ParticlePrefab({ 200,0,0 }, (range - 1)*15.0f, 1.f);
	rightParticle->GetTransform()->Translate(m_LevelGrid[x][y].Position.x, 7.5f, m_LevelGrid[x][y].Position.y);
	AddChild(rightParticle);
	m_pParticles.push_back(rightParticle);
	//TOP
	//***
	ParticlePrefab* topParticle = nullptr;
	for (int i = 0; i < range; ++i)
	{
		if (explodeAll)
		{
			if (y - (i + 1) < 0 || m_LevelGrid[x][y - (i + 1)].IsPartOfLevel)
			{
				topParticle = new ParticlePrefab({ 0,0,200 }, 2.5f + i * 15.0f, 1.f);
				break;
			}
		}
		else
		{
			if (y - (i + 1) < 0 || m_LevelGrid[x][y - (i + 1)].hasStone || m_LevelGrid[x][y - (i + 1)].IsPartOfLevel)
			{
				topParticle = new ParticlePrefab({ 0,0,200 }, 2.5f + i * 15.0f, 1.f);
				break;
			}
		}
	}
	if (topParticle == nullptr)
		topParticle = new ParticlePrefab({ 0,0,200 }, (range - 1)*15.0f, 1.f);
	topParticle->GetTransform()->Translate(m_LevelGrid[x][y].Position.x, 7.5f, m_LevelGrid[x][y].Position.y);
	AddChild(topParticle);
	m_pParticles.push_back(topParticle);
	//BOTTOM
	//******
	ParticlePrefab* bottomParticle = nullptr;
	for (int i = 0; i < range; ++i)
	{
		if (explodeAll)
		{
			if (y + (i + 1) < 0 || m_LevelGrid[x][y + (i + 1)].IsPartOfLevel)
			{
				bottomParticle = new ParticlePrefab({ 0,0,-200 }, 2.5f + i * 15.0f, 1.f);
				break;
			}
		}
		else
		{
			if (y + (i + 1) < 0 || m_LevelGrid[x][y + (i + 1)].hasStone || m_LevelGrid[x][y + (i + 1)].IsPartOfLevel)
			{
				bottomParticle = new ParticlePrefab({ 0,0,-200 }, 2.5f + i * 15.0f, 1.f);
				break;
			}
		}
	}
	if (bottomParticle == nullptr)
		bottomParticle = new ParticlePrefab({ 0,0,-200 }, (range - 1)*15.0f, 1.f);
	bottomParticle->GetTransform()->Translate(m_LevelGrid[x][y].Position.x, 7.5f, m_LevelGrid[x][y].Position.y);
	AddChild(bottomParticle);
	m_pParticles.push_back(bottomParticle);
}

void BomberManScene::GameOver()
{
	if (m_CurrentRound >= 3)
	{
		//RemoveChild(m_pCountDown, true);
		if (m_pUI[0]->GetPoints() == m_pUI[1]->GetPoints())
		{
			//TIE
			static_cast<BomberManGameOver*>(SceneManager::GetInstance()->GetScene(L"BomberManGameOver"))->SetWinState(tie);
		}
		else if (m_pUI[0]->GetPoints() > m_pUI[1]->GetPoints())
		{
			//PLAYER 1 WIN
			static_cast<BomberManGameOver*>(SceneManager::GetInstance()->GetScene(L"BomberManGameOver"))->SetWinState(player1);
		}
		else
		{
			//PLAYER 2 WIN
			static_cast<BomberManGameOver*>(SceneManager::GetInstance()->GetScene(L"BomberManGameOver"))->SetWinState(player2);
		}
		SceneManager::GetInstance()->SetActiveGameScene(L"BomberManGameOver");
	}
}

bool BomberManScene::HasExploded(GameObject* player, int x, int y, int range) const
{
	PVector2 playerIdx = GetGridIndex(player);
	if (playerIdx.x > x - range && playerIdx.x < x + range && playerIdx.y == y)
	{
		if (playerIdx.x < x - 1)
		{
			if (m_LevelGrid[static_cast<int>(playerIdx.x) + 1][static_cast<int>(playerIdx.y)].IsPartOfLevel)
			{
				return false;
			}
			if (static_cast<CharacterController*>(player)->HasLives())
			{
				static_cast<CharacterController*>(player)->SubLive();
				return false;
			}
			return true;
		}
		if (playerIdx.x > x + 1)
		{
			if (m_LevelGrid[static_cast<int>(playerIdx.x) - 1][static_cast<int>(playerIdx.y)].IsPartOfLevel)
			{
				return false;
			}
			if (static_cast<CharacterController*>(player)->HasLives())
			{
				static_cast<CharacterController*>(player)->SubLive();
				return false;
			}
			return true;
		}
		if (static_cast<CharacterController*>(player)->HasLives())
		{
			static_cast<CharacterController*>(player)->SubLive();
			return false;
		}
		return true;
	}
	if (playerIdx.y > y - range && playerIdx.y < y + range && playerIdx.x == x)
	{
		if (playerIdx.y < y - 1)
		{
			if (m_LevelGrid[static_cast<int>(playerIdx.x)][static_cast<int>(playerIdx.y) + 1].IsPartOfLevel)
			{
				return false;
			}
			if (static_cast<CharacterController*>(player)->HasLives())
			{
				static_cast<CharacterController*>(player)->SubLive();
				return false;
			}
			return true;
		}
		if (playerIdx.y > y + 1)
		{
			if (m_LevelGrid[static_cast<int>(playerIdx.x)][static_cast<int>(playerIdx.y - 1)].IsPartOfLevel)
			{
				return false;
			}
			if (static_cast<CharacterController*>(player)->HasLives())
			{
				static_cast<CharacterController*>(player)->SubLive();
				return false;
			}
			return true;
		}
		if (static_cast<CharacterController*>(player)->HasLives())
		{
			static_cast<CharacterController*>(player)->SubLive();
			return false;
		}
		return true;
	}
	return false;
}

PVector2 BomberManScene::GetGridIndex(GameObject* object) const
{
	float posX = object->GetTransform()->GetPosition().x;
	int x = 0;
	float initX = -75.f;
	for (int i = 0; i < 11; ++i)
	{
		if (posX > initX - 7.5f && posX < initX + 7.5f)
		{
			x = i; break;
		}
		initX += 15.0f;
	}
	float posY = object->GetTransform()->GetPosition().z;
	int y = 0;
	float initY = 75.f;
	for (int i = 0; i < 11; ++i)
	{
		if (posY < initY + 7.5f && posY > initY - 7.5f)
		{
			y = i; break;
		}
		initY -= 15.0f;
	}
	return PVector2{ static_cast<float>(x),static_cast<float>(y) };
}
