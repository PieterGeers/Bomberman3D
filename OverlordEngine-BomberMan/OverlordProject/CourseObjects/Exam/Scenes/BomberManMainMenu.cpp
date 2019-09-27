#include "stdafx.h"
#include "BomberManMainMenu.h"
#include "../BomberManStructs.h"
#include "GameObject.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "SceneManager.h"
#include "OverlordGame.h"
#include "SoundManager.h"

BomberManMainMenu::BomberManMainMenu()
	:GameScene(L"BomberManMainMenu")
{
}

void BomberManMainMenu::Initialize()
{
	const auto gameContext = GetGameContext();

	auto mainMenuText = new GameObject();
	auto spriteMM = new SpriteComponent(L"./Resources/Textures/BomberMan/MainMenu.png");
	mainMenuText->AddComponent(spriteMM);
	AddChild(mainMenuText);
	mainMenuText->GetTransform()->Translate(0, 0, 0.99f);

	auto ButtonPlay = new GameObject();
	auto ButtonPlayS = new SpriteComponent(L"./Resources/Textures/BomberMan/PlayButton.png", DirectX::XMFLOAT2(0.5f,0.5f));
	ButtonPlay->AddComponent(ButtonPlayS);
	AddChild(ButtonPlay);
	ButtonPlay->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	ButtonPlay->GetTransform()->Translate(640, 300, 0.98f);
	m_Buttons.push_back(ButtonPlay);

	auto ButtonPlay2 = new GameObject();
	auto ButtonPlayS2 = new SpriteComponent(L"./Resources/Textures/BomberMan/OptionsButton.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	ButtonPlay2->AddComponent(ButtonPlayS2);
	AddChild(ButtonPlay2);
	ButtonPlay2->GetTransform()->Scale(.75f, .75f, .75f);
	ButtonPlay2->GetTransform()->Translate(640, 450, 0.98f);
	m_Buttons.push_back(ButtonPlay2);

	auto ButtonPlay3 = new GameObject();
	auto ButtonPlayS3 = new SpriteComponent(L"./Resources/Textures/BomberMan/QuitButton.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	ButtonPlay3->AddComponent(ButtonPlayS3);
	AddChild(ButtonPlay3);
	ButtonPlay3->GetTransform()->Scale(.75f, .75f, .75f);
	ButtonPlay3->GetTransform()->Translate(640,600, 0.98f);
	m_Buttons.push_back(ButtonPlay3);

}

void BomberManMainMenu::Update()
{
	const auto gameContext = GetGameContext();
	if (gameContext.pInput->IsActionTriggered(UIButtonUp))
	{
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(m_PrevScale);
		--m_ActiveButton;
		if (m_ActiveButton < 0)
			m_ActiveButton = (m_Buttons.size() - 1);
		m_PrevScale = m_Buttons[m_ActiveButton]->GetTransform()->GetScale();
		if (m_ActiveButton == 0)
			m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
		else
			m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.f, 1.f, 1.f);
	}
	if (gameContext.pInput->IsActionTriggered(UIButtonDown))
	{
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(m_PrevScale);
		++m_ActiveButton;
		if (m_ActiveButton > static_cast<int>(m_Buttons.size() - 1))
			m_ActiveButton = 0;
		m_PrevScale = m_Buttons[m_ActiveButton]->GetTransform()->GetScale();
		if (m_ActiveButton == 0)
			m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
		else
			m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.f, 1.f, 1.f);
	}
	if (gameContext.pInput->IsActionTriggered(UIButtonSelect))
	{
		if (m_ActiveButton == 0)
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManScene");
		else if (m_ActiveButton == 1)
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManOptions");
		else if (m_ActiveButton == 2)
			OverlordGame::QuitGame();
	}
}

void BomberManMainMenu::Draw()
{
}

void BomberManMainMenu::SceneActivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Resume();
	const auto gameContext = GetGameContext();
	gameContext.pInput->AddInputAction(InputAction(UIButtonSelect, Pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonDown, Pressed, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonUp, Pressed, 'W', -1, XINPUT_GAMEPAD_DPAD_UP, PlayerOne));
}

void BomberManMainMenu::SceneDeactivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Pause();
	const auto gameContext = GetGameContext();
	gameContext.pInput->RemoveInputAction(UIButtonSelect);
	gameContext.pInput->RemoveInputAction(UIButtonDown);
	gameContext.pInput->RemoveInputAction(UIButtonUp);
}
