#include "stdafx.h"
#include "CountDownPrefab.h"
#include "ContentManager.h"
#include "SpriteComponent.h"
#include "GameScene.h"
#include "TransformComponent.h"
#include "Scenes/BomberManScene.h"
#include "SoundManager.h"

CountDownPrefab::CountDownPrefab()
{
	std::cout << "CountDownPrefab Constructor" << std::endl;
}

void CountDownPrefab::Initialize(const GameContext&)
{
	auto sprite = new SpriteComponent(L"./Resources/Textures/BomberMan/3.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	AddComponent(sprite);
	GetTransform()->Translate(640, 360, .99f);
}

void CountDownPrefab::PostInitialize(const GameContext&)
{
}

void CountDownPrefab::Draw(const GameContext&)
{
}

void CountDownPrefab::PostDraw(const GameContext&)
{
}

void CountDownPrefab::Update(const GameContext& gameContext)
{
	if (m_IsFreeze)
		return;
	if (m_CountDownComplete)
	{
		static_cast<BomberManScene*>(GetScene())->UnfreezePlayers();
		GetScene()->RemoveChild(this, true);
	}
	if (m_CountDown == 3 && m_AccTime == 0.0f)
	{
		SoundManager::GetInstance()->GetEffect("Countdown1")->Play();
	}
	m_AccTime += gameContext.pGameTime->GetElapsed();
	if (m_AccTime >= 1.0f)
	{
		--m_CountDown;
		if (m_CountDown == 2)
		{
			GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/BomberMan/2.png");
			SoundManager::GetInstance()->GetEffect("Countdown1")->Play();
		}
		else if (m_CountDown == 1)
		{
			GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/BomberMan/1.png");
			SoundManager::GetInstance()->GetEffect("Countdown1")->Play();
		}
		else if (m_CountDown == 0)
		{
			GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/BomberMan/Start.png");
			SoundManager::GetInstance()->GetEffect("Countdown2")->Play();
		}
		else if (m_CountDown == -1)
			m_CountDownComplete = true;
		m_AccTime = 0.0f;
	}
}

CountDownPrefab::~CountDownPrefab()
{
}
