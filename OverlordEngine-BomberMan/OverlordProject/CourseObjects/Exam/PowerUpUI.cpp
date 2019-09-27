#include "stdafx.h"
#include "PowerUpUI.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "ContentManager.h"
#include "GameScene.h"
#include "TextRenderer.h"

PowerUpUI::PowerUpUI(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 lifePosition, std::wstring refTag, bool isLeft)
	:m_Position(position)
	,m_LifePosition(lifePosition)
	,m_Tag(refTag)
	,m_IsLeft(isLeft)
{
}

void PowerUpUI::Restart()
{
	GetScene()->RemoveChild(m_Player, true);
	m_Player = new GameObject();
	if (m_IsLeft)
	{
		auto pic = new SpriteComponent(L"./Resources/Textures/BomberMan/1UI.png", DirectX::XMFLOAT2(0, 0));
		m_Player->AddComponent(pic);
		m_Player->GetTransform()->Translate(16, 16, .96f);
	}
	else
	{
		auto pic = new SpriteComponent(L"./Resources/Textures/BomberMan/2UI.png", DirectX::XMFLOAT2(1, 0));
		m_Player->AddComponent(pic);
		m_Player->GetTransform()->Translate(1264.f, 16.f, .96f);
	}
	m_Player->GetTransform()->Scale(.75f, .75f, .75f);
	GetScene()->AddChild(m_Player);
	m_IsFreeze = true;
	m_Point = 0;
}

void PowerUpUI::Initialize(const GameContext&)
{
	m_Character = static_cast<CharacterController*>(GetScene()->GetGameObjectWithTag(m_Tag));
	if (m_Character == nullptr)
		Logger::LogError(L"Character not found in UI");

	m_pFont = ContentManager::Load<SpriteFont>(L"./Resources/SpriteFonts/Bomberman_20.fnt");

	auto sprite = new SpriteComponent(L"./Resources/Textures/BomberMan/PowerupUI.png", DirectX::XMFLOAT2(0.5f, 0.5f));
	AddComponent(sprite);
	GetTransform()->Translate(m_Position);
	GetTransform()->Scale(.5f, .5f, .5f);

	m_Player = new GameObject();
	if (m_IsLeft)
	{
		auto pic = new SpriteComponent(L"./Resources/Textures/BomberMan/1UI.png", DirectX::XMFLOAT2(0, 0));
		m_Player->AddComponent(pic);
		m_Player->GetTransform()->Translate(16, 16, .96f);
	}
	else
	{
		auto pic = new SpriteComponent(L"./Resources/Textures/BomberMan/2UI.png", DirectX::XMFLOAT2(1, 0));
		m_Player->AddComponent(pic);
		m_Player->GetTransform()->Translate(1264.f, 16.f, .96f);
	}
	m_Player->GetTransform()->Scale(.75f, .75f, .75f);
	GetScene()->AddChild(m_Player);

	m_Select = new GameObject();
	auto select = new SpriteComponent(L"./Resources/Textures/BomberMan/PowerupUISelectNone.png", DirectX::XMFLOAT2(.5f, .5f));
	m_Select->AddComponent(select);
	m_Select->GetTransform()->Translate(0, 0, .99f);
	AddChild(m_Select);

	auto liveObject = new GameObject();
	auto live = new SpriteComponent(L"./Resources/Textures/BomberMan/PU_live.png", DirectX::XMFLOAT2(.5f, .5f));
	liveObject->AddComponent(live);
	liveObject->GetTransform()->Translate(m_LifePosition);
	AddChild(liveObject);

	auto slowObject = new GameObject();
	auto slow = new SpriteComponent(L"./Resources/Textures/BomberMan/PU_slow.png", DirectX::XMFLOAT2(.5f, .5f));
	slowObject->AddComponent(slow);
	slowObject->GetTransform()->Translate(m_LifePosition.x, m_LifePosition.y + 160, m_LifePosition.z);
	AddChild(slowObject);
}

void PowerUpUI::PostInitialize(const GameContext&)
{
}

void PowerUpUI::Draw(const GameContext&)
{
	if (m_IsFreeze)
		return;
	//RANGE
	//*****
	TextRenderer::GetInstance()->DrawText(m_pFont, std::to_wstring(m_Character->GetRange()), DirectX::XMFLOAT2(m_Position.x + 21, m_Position.y - 70), DirectX::XMFLOAT4(DirectX::Colors::Black));
	//ULTIMATE
	//********
	TextRenderer::GetInstance()->DrawText(m_pFont, std::to_wstring(m_Character->GetUltimate()), DirectX::XMFLOAT2(m_Position.x + 21, m_Position.y + 105), DirectX::XMFLOAT4(DirectX::Colors::Black));
	//POWER
	//*****
	TextRenderer::GetInstance()->DrawText(m_pFont, std::to_wstring(m_Character->GetPower()), DirectX::XMFLOAT2(m_Position.x + 110, m_Position.y + 17), DirectX::XMFLOAT4(DirectX::Colors::Black));
	//REMOTE
	//******
	TextRenderer::GetInstance()->DrawText(m_pFont, std::to_wstring(m_Character->GetRemote()), DirectX::XMFLOAT2(m_Position.x - 70, m_Position.y + 17), DirectX::XMFLOAT4(DirectX::Colors::Black));
	//LIVES
	//*****
	if (m_IsLeft)
		TextRenderer::GetInstance()->DrawText(m_pFont, std::to_wstring(m_Character->GetLives()), DirectX::XMFLOAT2( m_Position.x - 64, m_Position.y - 235), DirectX::XMFLOAT4(DirectX::Colors::Black));
	else
		TextRenderer::GetInstance()->DrawText(m_pFont, std::to_wstring(m_Character->GetLives()), DirectX::XMFLOAT2(m_Position.x + 48, m_Position.y - 235), DirectX::XMFLOAT4(DirectX::Colors::Black));
	//SLOW
	//****
	if (m_IsLeft)
		TextRenderer::GetInstance()->DrawText(m_pFont, std::to_wstring(m_Character->GetSlowTime()), DirectX::XMFLOAT2(m_Position.x - 64, m_Position.y - 155), DirectX::XMFLOAT4(DirectX::Colors::Black));
	else
		TextRenderer::GetInstance()->DrawText(m_pFont, std::to_wstring(m_Character->GetSlowTime()), DirectX::XMFLOAT2(m_Position.x + 48, m_Position.y - 155), DirectX::XMFLOAT4(DirectX::Colors::Black));
	//PLAYER
	//******
	if (m_IsLeft)
		TextRenderer::GetInstance()->DrawText(m_pFont, L"Player 1:", DirectX::XMFLOAT2(30, 25), DirectX::XMFLOAT4(DirectX::Colors::Black));
	else
		TextRenderer::GetInstance()->DrawText(m_pFont, L"Player 2:", DirectX::XMFLOAT2(1000, 25), DirectX::XMFLOAT4(DirectX::Colors::Black));

}

void PowerUpUI::PostDraw(const GameContext&)
{
}

void PowerUpUI::Update(const GameContext&)
{
}

PowerUpUI::~PowerUpUI()
{
}

void PowerUpUI::ResetSelected()
{
	m_Select->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/BomberMan/PowerupUISelectNone.png");
}

void PowerUpUI::RotateSelected(float angle)
{
	m_Select->GetComponent<SpriteComponent>()->SetTexture(L"./Resources/Textures/BomberMan/PowerupUISelect.png");
	m_Select->GetTransform()->Rotate(0, 0, angle);
}

void PowerUpUI::AddPoint()
{
	auto point = new GameObject();
	auto pic = new SpriteComponent(L"./Resources/Textures/BomberMan/Point.png");
	point->AddComponent(pic);
	m_Player->AddChild(point);
	point->GetTransform()->Scale(.3f, .3f, .3f);
	if (m_IsLeft)
		point->GetTransform()->Translate( 190 + m_Point * 55.0f, 10.0f, .95f);
	else
		point->GetTransform()->Translate(-180 + m_Point * 55.0f, 10.0f, .95f);
	++m_Point;
} 
