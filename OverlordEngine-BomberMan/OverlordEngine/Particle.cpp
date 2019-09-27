#include "stdafx.h"
#include "Particle.h"

// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
using namespace DirectX;

Particle::Particle(const ParticleEmitterSettings& emitterSettings):
	m_VertexInfo(ParticleVertex()),
	m_EmitterSettings(emitterSettings),
	m_IsActive(false),
	m_TotalEnergy(0),
	m_CurrentEnergy(0),
	m_SizeGrow(0),
	m_InitSize(0)
{}

void Particle::Update(const GameContext& gameContext)
{
	if (!m_IsActive)
		return;
	m_CurrentEnergy -= gameContext.pGameTime->GetElapsed();
	if (m_CurrentEnergy < 0.0f)
	{
		m_IsActive = false;
		return;
	}
	if (Distance(m_InitPosition, m_VertexInfo.Position) > m_MaxTravelDistance)
		m_IsActive = false;
	//Position
	const XMVECTOR velocity = XMLoadFloat3(&m_EmitterSettings.Velocity) * gameContext.pGameTime->GetElapsed();
	XMStoreFloat3(&m_VertexInfo.Position, XMVectorAdd(velocity, XMLoadFloat3(&m_VertexInfo.Position)));
	//color
	m_VertexInfo.Color = m_EmitterSettings.Color;
	const float particleLifePercent = m_CurrentEnergy / m_TotalEnergy;
	m_VertexInfo.Color.w = particleLifePercent * 2;
	//size
	m_VertexInfo.Size = m_InitSize * (1 + ((m_SizeGrow - 1) * (1 - particleLifePercent)));
}

void Particle::Init(XMFLOAT3 initPosition, float range)
{
	m_IsActive = true;
	m_TotalEnergy = m_CurrentEnergy = randF(m_EmitterSettings.MinEnergy, m_EmitterSettings.MaxEnergy);
	m_InitPosition = initPosition;
	m_MaxTravelDistance = range;
	//position
	XMVECTOR randomDirection{ 1,0,0 };
	const XMMATRIX randomRotationMatrix = XMMatrixRotationRollPitchYaw(randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI), randF(-XM_PI, XM_PI));
	randomDirection = XMVector3TransformNormal(randomDirection, randomRotationMatrix);
	const float distance = randF(m_EmitterSettings.MinEmitterRange, m_EmitterSettings.MaxEmitterRange);
	XMStoreFloat3(&m_VertexInfo.Position,XMLoadFloat3(&initPosition) + (randomDirection * distance));
	//size
	m_VertexInfo.Size = m_InitSize = randF(m_EmitterSettings.MinSize, m_EmitterSettings.MaxSize);
	m_SizeGrow = randF(m_EmitterSettings.MinSizeGrow, m_EmitterSettings.MaxSizeGrow);
	//rotation
	m_VertexInfo.Rotation = randF(-XM_PI, XM_PI);
}

float Particle::Distance(const XMFLOAT3& v1, const XMFLOAT3& v2)
{
	XMVECTOR vector1 = XMLoadFloat3(&v1);
	XMVECTOR vector2 = XMLoadFloat3(&v2);
	XMVECTOR vectorSub = XMVectorSubtract(vector1, vector2);
	XMVECTOR length = XMVector3Length(vectorSub);

	float distance = 0.0f;
	XMStoreFloat(&distance, length);
	return distance;
}