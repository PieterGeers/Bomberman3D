#include "stdafx.h"
#include "BomberManSettings.h"
#include "../BomberManStructs.h"
#include "SoundManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "SceneManager.h"

BomberManSettings::BomberManSettings()
	:GameScene(L"BomberManSettings")
{
}


BomberManSettings::~BomberManSettings()
{
}

void BomberManSettings::Initialize()
{
	auto background = new GameObject();
	auto backgroundtex = new SpriteComponent(L"./Resources/Textures/BomberMan/SettingsScreen.png");
	background->AddComponent(backgroundtex);
	AddChild(background);
	background->GetTransform()->Translate(0.0f, 0.0f, .99f);

	auto soundEffect = new GameObject();
	auto setex = new SpriteComponent(L"./Resources/Textures/BomberMan/soundeffects.png", {0.f,.5f});
	soundEffect->AddComponent(setex);
	AddChild(soundEffect);
	soundEffect->GetTransform()->Translate(16.f, 200.f, .98f);

	auto soundStream = new GameObject();
	auto sstex = new SpriteComponent(L"./Resources/Textures/BomberMan/soundstream.png", { 0.f,.5f });
	soundStream->AddComponent(sstex);
	AddChild(soundStream);
	soundStream->GetTransform()->Translate(16.f, 300.f, .98f);

	auto buttonsmin1 = new GameObject();
	auto bm1tex = new SpriteComponent(L"./Resources/Textures/BomberMan/min.png", { 0.5f,.5f });
	buttonsmin1->AddComponent(bm1tex);
	AddChild(buttonsmin1);
	buttonsmin1->GetTransform()->Translate(420.0f, 200.0f, .98f);
	m_Buttons.push_back(buttonsmin1);

	auto buttonsplus1 = new GameObject();
	auto bp1tex = new SpriteComponent(L"./Resources/Textures/BomberMan/plus.png", { .5f,.5f });
	buttonsplus1->AddComponent(bp1tex);
	AddChild(buttonsplus1);
	buttonsplus1->GetTransform()->Scale(.8f, .8f, .8f);
	buttonsplus1->GetTransform()->Translate(1164.0f, 200, .98f);
	m_Buttons.push_back(buttonsplus1);

	auto buttonsmin2 = new GameObject();
	auto bm2tex = new SpriteComponent(L"./Resources/Textures/BomberMan/min.png", { 0.5f,.5f });
	buttonsmin2->AddComponent(bm2tex);
	AddChild(buttonsmin2);
	buttonsmin2->GetTransform()->Scale(.8f, .8f, .8f);
	buttonsmin2->GetTransform()->Translate(420.0f, 300.0f, .98f);
	m_Buttons.push_back(buttonsmin2);

	auto buttonsplus2 = new GameObject();
	auto bp2tex = new SpriteComponent(L"./Resources/Textures/BomberMan/plus.png", { .5f,.5f });
	buttonsplus2->AddComponent(bp2tex);
	AddChild(buttonsplus2);
	buttonsplus2->GetTransform()->Scale(.8f, .8f, .8f);
	buttonsplus2->GetTransform()->Translate(1164.0f, 300, .98f);
	m_Buttons.push_back(buttonsplus2);

	auto backbutton = new GameObject();
	auto backtex = new SpriteComponent(L"./Resources/Textures/BomberMan/Back.png", { .5f,.5f });
	backbutton->AddComponent(backtex);
	AddChild(backbutton);
	backbutton->GetTransform()->Translate(1100.0f, 550.0f, .98f);
	m_Buttons.push_back(backbutton);

	for (int i = 0; i < 5; ++i)
	{
		auto bar = new GameObject();
		auto barTex = new SpriteComponent(L"./Resources/Textures/BomberMan/soundBar.png", { .5f,.5f });
		bar->AddComponent(barTex);
		AddChild(bar);
		bar->GetTransform()->Translate(static_cast<float>(525 + i * 60), 200.0f, .985f);
		m_SoundEffectVolumeBar.push_back(bar);
	}

	for (int i = 0; i < 2; ++i)
	{
		auto bar = new GameObject();
		auto barTex = new SpriteComponent(L"./Resources/Textures/BomberMan/soundBar.png", { .5f,.5f });
		bar->AddComponent(barTex);
		AddChild(bar);
		bar->GetTransform()->Translate(static_cast<float>(525 + i * 60), 300.0f, .985f);
		m_SoundStreamVolumeBar.push_back(bar);
	}

}

void BomberManSettings::Update()
{
	auto gameContext = GetGameContext();
	if (gameContext.pInput->IsActionTriggered(UIButtonUp))
	{
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(m_PrevScale);
		--m_ActiveButton;
		if (m_ActiveButton < 0)
			m_ActiveButton = (m_Buttons.size() - 1);
		m_PrevScale = m_Buttons[m_ActiveButton]->GetTransform()->GetScale();
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.f, 1.f, 1.f);
		if (m_ActiveButton == 4)
			m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	}
	if (gameContext.pInput->IsActionTriggered(UIButtonDown))
	{
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(m_PrevScale);
		++m_ActiveButton;
		if (m_ActiveButton > static_cast<int>(m_Buttons.size() - 1))
			m_ActiveButton = 0;
		m_PrevScale = m_Buttons[m_ActiveButton]->GetTransform()->GetScale();
		m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.f, 1.f, 1.f);
		if (m_ActiveButton == 4)
			m_Buttons[m_ActiveButton]->GetTransform()->Scale(1.2f, 1.2f, 1.2f);
	}
	if (gameContext.pInput->IsActionTriggered(UIButtonSelect))
	{
		if (m_ActiveButton == 0)
		{
			//SOUND EFFECT MIN
			SoundManager::GetInstance()->ChangeVolumeSoundEffect(-.1f);
			if (!m_SoundEffectVolumeBar.empty())
			{
				RemoveChild(*(m_SoundEffectVolumeBar.end()-1));
				m_SoundEffectVolumeBar.pop_back();
			}
		}
		else if (m_ActiveButton == 1)
		{
			//SOUND EFFECT PLUS
			SoundManager::GetInstance()->ChangeVolumeSoundEffect(.1f);
			if (m_SoundEffectVolumeBar.size() < unsigned(10))
			{
				auto bar = new GameObject();
				auto barTex = new SpriteComponent(L"./Resources/Textures/BomberMan/soundBar.png", { .5f,.5f });
				bar->AddComponent(barTex);
				AddChild(bar);
				bar->GetTransform()->Translate(static_cast<float>(525 + m_SoundEffectVolumeBar.size() * 60), 200.0f, .985f);
				m_SoundEffectVolumeBar.push_back(bar);
			}
		}
		else if (m_ActiveButton == 2)
		{
			//SOUND STREAM MIN
			SoundManager::GetInstance()->ChangeVolumeSoundStream(-.1f);
			if (!m_SoundStreamVolumeBar.empty())
			{
				RemoveChild(*(m_SoundStreamVolumeBar.end()-1));
				m_SoundStreamVolumeBar.pop_back();
			}
		}
		else if (m_ActiveButton == 3)
		{
			//SOUND STREAM PLUS	
			SoundManager::GetInstance()->ChangeVolumeSoundStream(.1f);
			if (m_SoundStreamVolumeBar.size() < unsigned(10))
			{
				auto bar = new GameObject();
				auto barTex = new SpriteComponent(L"./Resources/Textures/BomberMan/soundBar.png", { .5f,.5f });
				bar->AddComponent(barTex);
				AddChild(bar);
				bar->GetTransform()->Translate(static_cast<float>(525 + m_SoundStreamVolumeBar.size() * 60), 300.0f, .985f);
				m_SoundStreamVolumeBar.push_back(bar);
			}

		}
		else if (m_ActiveButton == 4)
		{
			SceneManager::GetInstance()->SetActiveGameScene(L"BomberManOptions");
		}

	}
}

void BomberManSettings::Draw()
{
}

void BomberManSettings::SceneActivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Resume();

	const auto gameContext = GetGameContext();
	gameContext.pInput->AddInputAction(InputAction(UIButtonSelect, Pressed, VK_SPACE, -1, XINPUT_GAMEPAD_A, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonDown, Pressed, 'S', -1, XINPUT_GAMEPAD_DPAD_DOWN, PlayerOne));
	gameContext.pInput->AddInputAction(InputAction(UIButtonUp, Pressed, 'W', -1, XINPUT_GAMEPAD_DPAD_UP, PlayerOne));
}

void BomberManSettings::SceneDeactivated()
{
	SoundManager::GetInstance()->GetStream("Menu")->Pause();

	const auto gameContext = GetGameContext();
	gameContext.pInput->RemoveInputAction(UIButtonSelect);
	gameContext.pInput->RemoveInputAction(UIButtonDown);
	gameContext.pInput->RemoveInputAction(UIButtonUp);
}
