#include "stdafx.h"
#include "BomberManInGamePauseMenu.h"
#include "../BomberManStructs.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "SpriteComponent.h"
#include "OverlordGame.h"
#include "BomberManOptions.h"
#include "BomberManScene.h"
#include "SoundManager.h"

BomberManInGamePauseMenu::BomberManInGamePauseMenu()
	:GameScene(L"BomberManInGamePauseMenu")
{
}


BomberManInGamePauseMenu::~BomberManInGamePauseMenu()
{

}

void BomberManInGamePauseMenu::Initialize()
{
	auto powerups = new GameObject();
	auto spritePU = new SpriteComponent(L"./Resources/Textures/BomberMan/background.png");
	powerups->AddComponent(spritePU);
	AddChild(powerups);
	powerups->GetTransform()->Translate(0, 0, 0.99f);

	auto mainmenu = new GameObject();
	auto spritemm = new SpriteComponent(L"./Resources/Textures/BomberMan/MainMenuButton.png", {.5,.5});
	mainmenu->AddComponent(spritemm);
	AddChild(mainmenu);
	mainmenu->GetTransform()->Translate(640, 144, 0.97f);
	mainmenu->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	m_Buttons.push_back(mainmenu);

	auto Restart = new GameObject();
	auto RestartTex = new SpriteComponent(L"./Resources/Textures/BomberMan/Restart.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	Restart->AddComponent(RestartTex);
	AddChild(Restart);
	Restart->GetTransform()->Translate(640, 288, 0.97f);
	m_Buttons.push_back(Restart);

	auto Options = new GameObject();
	auto OptionsTex = new SpriteComponent(L"./Resources/Textures/BomberMan/OptionsButton.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	Options->AddComponent(OptionsTex);
	AddChild(Options);
	Options->GetTransform()->Translate(640, 432, 0.97f);
	m_Buttons.push_back(Options);

	auto Quit = new GameObject();
	auto QuitTex = new SpriteComponent(L"./Resources/Textures/BomberMan/QuitButton.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	Quit->AddComponent(QuitTex);
	AddChild(Quit);
	Quit->GetTransform()->Translate(640, 576, 0.97f);
	m_Buttons.push_back(Quit);

}

void BomberManInGamePauseMenu::Update()
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
			//RESET GAME
			static_cast<BomberManScene*>(SceneManager::GetInstance()->GetScene(L"BomberManScene"))->Restart();
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManMainMenu");
		}
		else if (m_ActiveButton == 1)
		{
			//RESET GAME
			static_cast<BomberManScene*>(SceneManager::GetInstance()->GetScene(L"BomberManScene"))->Restart();
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
		}
		else if (m_ActiveButton == 2)
		{
			//SET OPTIONS BACK COMMAND
			static_cast<BomberManOptions*>(SceneManager::GetInstance()->GetScene(L"BomberManOptions"))->SetBackToInGame();
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManOptions");
		}
		else if (m_ActiveButton == 3)
		{
			//QUIT GAME
			OverlordGame::QuitGame();
		}
	}

}

void BomberManInGamePauseMenu::Draw()
{

}

void BomberManInGamePauseMenu::SceneActivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Resume();

	const auto gameContext = GetGameContext();
	gameContext.pInput->AddInputAction(InputAction(Pause_P1, Pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_BACK, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonSelect, Pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonDown, Pressed, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonUp, Pressed, 'W', -1, XINPUT_GAMEPAD_DPAD_UP, PlayerOne));
}

void BomberManInGamePauseMenu::SceneDeactivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Pause();

	const auto gameContext = GetGameContext();
	gameContext.pInput->RemoveInputAction(UIButtonSelect);
	gameContext.pInput->RemoveInputAction(UIButtonDown);
	gameContext.pInput->RemoveInputAction(UIButtonUp);
	gameContext.pInput->RemoveInputAction(Pause_P1);
}
