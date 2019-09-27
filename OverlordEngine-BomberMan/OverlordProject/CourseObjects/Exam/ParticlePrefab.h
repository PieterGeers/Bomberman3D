#pragma once
#include "GameObject.h"
#include "ParticleEmitterComponent.h"

class ParticlePrefab final : public GameObject
{
public:
	ParticlePrefab(DirectX::XMFLOAT3 direction, float range, float timeAlive = 1.0f);
	~ParticlePrefab();

	bool GetIsAlive() const { return m_IsAlive; }
	void Destroy();
protected:
	void Initialize(const GameContext&) override;
	void PostInitialize(const GameContext&) override;
	void Draw(const GameContext&) override;
	void PostDraw(const GameContext&) override;
	void Update(const GameContext&) override;

private:
	DirectX::XMFLOAT3 m_Direction;
	float m_MaxRange = 0.0f;
	float m_TimeAlive = 1.0f;
	bool m_IsAlive = true;
};

