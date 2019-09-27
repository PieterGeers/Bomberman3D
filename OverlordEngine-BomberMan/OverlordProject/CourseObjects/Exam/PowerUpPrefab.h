#pragma once
#include "GameObject.h"

class PowerUpPrefab final : public GameObject
{
public:
	PowerUpPrefab(DirectX::XMFLOAT3 position);
	~PowerUpPrefab();
	
	bool GetHasBeenPickedUp() const { return m_Picked; }
	void SetPickedUp() { m_Picked = true; }

protected:
	void Initialize(const GameContext&) override;
	void PostInitialize(const GameContext&) override;
	void Draw(const GameContext&) override;
	void PostDraw(const GameContext&) override;
	void Update(const GameContext&) override;

protected:
	bool m_Picked = false;
	float m_Angle = 0;
	DirectX::XMFLOAT3 m_Position;
};

