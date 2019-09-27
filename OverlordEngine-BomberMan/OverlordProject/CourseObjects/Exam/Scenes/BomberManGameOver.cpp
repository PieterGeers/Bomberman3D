#include "stdafx.h"
#include "BomberManGameOver.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "BomberManScene.h"
#include "OverlordGame.h"
#include "SoundManager.h"

BomberManGameOver::BomberManGameOver()
	:GameScene(L"BomberManGameOver")
{
}

BomberManGameOver::~BomberManGameOver()
{
}

void BomberManGameOver::Initialize()
{
	auto object = new GameObject();
	auto sprite = new SpriteComponent(L"./Resources/Textures/BomberMan/GameOver.png");
	object->AddComponent(sprite);
	AddChild(object);
	object->GetTransform()->Translate(0.0f, 0.0f, .99f);

	auto mainmenu = new GameObject();
	auto spritemm = new SpriteComponent(L"./Resources/Textures/BomberMan/MainMenuButton.png", { .5,.5 });
	mainmenu->AddComponent(spritemm);
	AddChild(mainmenu);
	mainmenu->GetTransform()->Translate(640, 400, 0.97f);
	mainmenu->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	m_Buttons.push_back(mainmenu);

	auto Quit = new GameObject();
	auto QuitTex = new SpriteComponent(L"./Resources/Textures/BomberMan/QuitButton.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	Quit->AddComponent(QuitTex);
	AddChild(Quit);
	Quit->GetTransform()->Translate(640, 550, 0.97f);
	m_Buttons.push_back(Quit);

}

void BomberManGameOver::Update()
{
	const auto gameContext = GetGameContext();
	if (gameContext.pInput->IsActionTriggered(Pause_P1))
	{
		SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
	}

	if (gameContext.pInput->IsActionTriggered(UIButtonUp))
	{
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(m_PrevScale);
		--m_ActiveButton;
		if (m_ActiveButton < 0)
			m_ActiveButton = (m_Buttons.size() - 1);
		m_PrevScale = m_Buttons[m_ActiveButton]->GetTransform()->GetScale();
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	}
	if (gameContext.pInput->IsActionTriggered(UIButtonDown))
	{
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(m_PrevScale);
		++m_ActiveButton;
		if (m_ActiveButton > static_cast<int>(m_Buttons.size() - 1))
			m_ActiveButton = 0;
		m_PrevScale = m_Buttons[m_ActiveButton]->GetTransform()->GetScale();
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	}
	if (gameContext.pInput->IsActionTriggered(UIButtonSelect))
	{
		if (m_ActiveButton == 0)
		{
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManMainMenu");
		}
		else if (m_ActiveButton == 1)
		{
			OverlordGame::QuitGame();
		}
	}
}

void BomberManGameOver::Draw()
{
}

void BomberManGameOver::SceneActivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Resume();

	m_pWinObject = new GameObject();
	SpriteComponent* sprite = new SpriteComponent(L"./Resources/Textures/BomberMan/Tie.png", { .5f,.5f });
	switch (m_State)
	{
	case player1: 
		sprite->SetTexture(L"./Resources/Textures/BomberMan/Player1Win.png");
		m_pWinObject->AddComponent(sprite);
		break;
	case player2: 
		sprite->SetTexture(L"./Resources/Textures/BomberMan/Player2Win.png");
		m_pWinObject->AddComponent(sprite);
		break;
	case tie:
		m_pWinObject->AddComponent(sprite);
		break;
	default: ;
	}
	AddChild(m_pWinObject);
	m_pWinObject->GetTransform()->Translate(640.0f, 250.0f, .96f);
	static_cast<BomberManScene*>(SceneManager::GetInstance()->GetScene(L"BomberManScene"))->Restart();

	auto gameContext = GetGameContext();
	gameContext.pInput->AddInputAction(InputAction(UIButtonSelect, Pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonDown, Pressed, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonUp, Pressed, 'W', -1, XINPUT_GAMEPAD_DPAD_UP, PlayerOne));
}

void BomberManGameOver::SceneDeactivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Pause();

	const auto gameContext = GetGameContext();
	gameContext.pInput->RemoveInputAction(UIButtonSelect);
	gameContext.pInput->RemoveInputAction(UIButtonDown);
	gameContext.pInput->RemoveInputAction(UIButtonUp);
	RemoveChild(m_pWinObject, true);
}
