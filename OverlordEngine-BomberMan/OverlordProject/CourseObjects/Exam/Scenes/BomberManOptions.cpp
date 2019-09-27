#include "stdafx.h"
#include "BomberManOptions.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "../BomberManStructs.h"
#include "SceneManager.h"
#include "SoundManager.h"

BomberManOptions::BomberManOptions()
	:GameScene(L"BomberManOptions")
{
}


BomberManOptions::~BomberManOptions()
{
}

void BomberManOptions::Initialize()
{
	auto powerups = new GameObject();
	auto spritePU = new SpriteComponent(L"./Resources/Textures/BomberMan/powerups.png");
	powerups->AddComponent(spritePU);
	AddChild(powerups);
	powerups->GetTransform()->Translate(0, 0, 0.99f);

	auto ButtonSettings = new GameObject();
	auto ButtonPlayS = new SpriteComponent(L"./Resources/Textures/BomberMan/Settings.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	ButtonSettings->AddComponent(ButtonPlayS);
	AddChild(ButtonSettings);
	ButtonSettings->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	ButtonSettings->GetTransform()->Translate(1000, 270, 0.98f);
	m_Buttons.push_back(ButtonSettings);

	auto ButtonBack = new GameObject();
	auto ButtonPlayS2 = new SpriteComponent(L"./Resources/Textures/BomberMan/Back.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	ButtonBack->AddComponent(ButtonPlayS2);
	AddChild(ButtonBack);
	ButtonBack->GetTransform()->Translate(1000, 450, 0.98f);
	m_Buttons.push_back(ButtonBack);
}

void BomberManOptions::Update()
{
	const auto gameContext = GetGameContext();
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
			//SETTINGS
			m_GoToSetting = true;
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManSettings");
		}
		if (m_ActiveButton == 1)
		{
			if (!m_BackToInGame)
				SceneManager::GetInstance()->SetActiveGameScene(L"BomberManMainMenu");
			else
				SceneManager::GetInstance()->SetActiveGameScene(L"BomberManInGamePauseMenu");
		}
	}
	if (m_BackToInGame)
	{
		if (gameContext.pInput->IsActionTriggered(Pause_P1))
		{
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
		}
	}
}

void BomberManOptions::Draw()
{
}

void BomberManOptions::SceneActivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Resume();

	const auto gameContext = GetGameContext();
	if (m_BackToInGame)
		gameContext.pInput->AddInputAction(InputAction(Pause_P1, Pressed, VK_ESCAPE, -1, XINPUT_GAMEPAD_BACK, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonSelect, Pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonDown, Pressed, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonUp, Pressed, 'W', -1, XINPUT_GAMEPAD_DPAD_UP, PlayerOne));
}

void BomberManOptions::SceneDeactivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Pause();

	const auto gameContext = GetGameContext();
	if (m_BackToInGame)
		gameContext.pInput->RemoveInputAction(Pause_P1);
	if (!m_GoToSetting)
		m_BackToInGame = false;
	gameContext.pInput->RemoveInputAction(UIButtonSelect);
	gameContext.pInput->RemoveInputAction(UIButtonDown);
	gameContext.pInput->RemoveInputAction(UIButtonUp);
}
