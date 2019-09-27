#include "stdafx.h"
#include "ParticlePrefab.h"
#include "ParticleEmitterComponent.h"
#include "GameScene.h"

ParticlePrefab::ParticlePrefab(DirectX::XMFLOAT3 direction, float range, float timeAlive)
	:m_Direction(direction)
	,m_MaxRange(range)
	,m_TimeAlive(timeAlive)
{
}

ParticlePrefab::~ParticlePrefab()
{
}

void ParticlePrefab::Destroy()
{
	GetScene()->RemoveChild(this, true);
}

void ParticlePrefab::Initialize(const GameContext&)
{
	auto m_pParticleEmitter = new ParticleEmitterComponent(L"./Resources/Textures/BomberMan/FireBall.png", 60);
	m_pParticleEmitter->SetVelocity(m_Direction);
	m_pParticleEmitter->SetMaxTravelRange(m_MaxRange);
	m_pParticleEmitter->SetMinSize(10.0f);
	m_pParticleEmitter->SetMaxSize(15.0f);
	m_pParticleEmitter->SetMinEnergy(1.0f);
	m_pParticleEmitter->SetMaxEnergy(2.0f);
	m_pParticleEmitter->SetMinSizeGrow(1.0f);
	m_pParticleEmitter->SetMaxSizeGrow(1.0f);
	m_pParticleEmitter->SetMinEmitterRange(0.2f);
	m_pParticleEmitter->SetMaxEmitterRange(0.5f);
	m_pParticleEmitter->SetColor(DirectX::XMFLOAT4(1.f, 1.f, 1.f, 0.6f));
	AddComponent(m_pParticleEmitter);
}

void ParticlePrefab::PostInitialize(const GameContext&)
{
}

void ParticlePrefab::Draw(const GameContext&)
{
}

void ParticlePrefab::PostDraw(const GameContext&)
{
}

void ParticlePrefab::Update(const GameContext& gameContext)
{
	if (m_IsAlive)
	{
		m_TimeAlive -= gameContext.pGameTime->GetElapsed();
		if (m_TimeAlive <= 0.0f)
		{
			m_IsAlive = false;
		}
	}
}
