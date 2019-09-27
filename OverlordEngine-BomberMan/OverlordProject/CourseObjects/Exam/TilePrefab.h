#pragma once
#include "GameObject.h"
#include "Pmath.h"

class TilePrefab final : public GameObject
{
public:
	TilePrefab(PVector2 position);
	~TilePrefab();

	void SpawnPowerUp();

protected:
	void Initialize(const GameContext&) override;
	void PostInitialize(const GameContext&) override;
	void Draw(const GameContext&) override;
	void PostDraw(const GameContext&) override;
	void Update(const GameContext&) override;

private:
	PVector2 m_Position{};
};

