#pragma once
#include "GameObject.h"
#include "BomberManStructs.h"

class Bom final : public GameObject
{
public:
	Bom(bool isRemote = false);
	~Bom();

	bool HasBomExploded() const { return m_HasExploded; }
	static std::vector<Bom*>& GetBombsToRemove() { return m_pBombs; }

	void SetBlocksToDestroy(std::vector<GameObject*>);
	bool GetIsExploded() const { return m_HasExploded; }

	void SetType(PowerUp type) { m_BomType = type; }
	PowerUp GetType() const { return m_BomType; }

protected:
	void Initialize(const GameContext&) override;
	void PostInitialize(const GameContext&) override;
	void Draw(const GameContext&) override;
	void PostDraw(const GameContext&) override;
	void Update(const GameContext&) override;
	
private:
	float m_TimeBeforeKaboom = 1.0f;
	bool m_HasExploded = false;
	bool m_IsRemote = false;
	static std::vector<Bom*> m_pBombs;
	std::vector<GameObject*> m_Tiles{};
	PowerUp m_BomType = PowerUp::none;
};

