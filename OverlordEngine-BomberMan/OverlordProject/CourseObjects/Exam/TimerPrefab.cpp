#include "stdafx.h"
#include "TimerPrefab.h"
#include "SpriteFont.h"
#include "ContentManager.h"
#include "TextRenderer.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"

TimerPrefab::TimerPrefab(DirectX::XMFLOAT3 position, float time)
	:m_Position(position)
	,m_Time(time)
{}

TimerPrefab::~TimerPrefab()
{}

void TimerPrefab::Initialize(const GameContext&)
{
	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Consolas_32.fnt");
	auto Timer = new SpriteComponent(L"./Resources/Textures/BomberMan/Timer.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	AddComponent(Timer);
	GetTransform()->Scale(1.0f, 1.2f, 1.0f);
	GetTransform()->Translate(m_Position);
}

void TimerPrefab::PostInitialize(const GameContext&)
{}

void TimerPrefab::Draw(const GameContext&)
{
	std::wstringstream text;
	int time = static_cast<int>(m_Time);
	if (time % 60 >= 10)
		text << time / 60 << " : " << time % 60;
	else
		text << time / 60 << " : 0" << time % 60;
	TextRenderer::GetInstance()->DrawText(m_pFont, text.str(), DirectX::XMFLOAT2(600, 10), DirectX::XMFLOAT4(DirectX::Colors::White));
}

void TimerPrefab::PostDraw(const GameContext&)
{}

void TimerPrefab::Update(const GameContext& gameContext)
{
	if (!m_Paused)
	{
		m_Time -= gameContext.pGameTime->GetElapsed();
		if (m_Time <= 0.0f)
		{
			m_GameOver = true;
			m_Paused = true;
		}
	}
}
