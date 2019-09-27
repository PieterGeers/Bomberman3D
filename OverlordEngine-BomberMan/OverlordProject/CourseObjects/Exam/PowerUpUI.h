#pragma once
#include "GameObject.h"
#include "SpriteFont.h"
#include "CharacterController.h"

class PowerUpUI final : public GameObject
{
public:
	PowerUpUI(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 lifePosition, std::wstring refTag, bool isLeft = true);
	~PowerUpUI();

	void ResetSelected();
	void RotateSelected(float angle);

	void AddPoint();

	void Freeze() { m_IsFreeze = true; }
	void UnFreeze() { m_IsFreeze = false; }
	int GetPoints() const { return m_Point; }
	void Restart();

protected:
	void Initialize(const GameContext&) override;
	void PostInitialize(const GameContext&) override;
	void Draw(const GameContext&) override;
	void PostDraw(const GameContext&) override;
	void Update(const GameContext&) override;

private:
	DirectX::XMFLOAT3 m_Position;
	DirectX::XMFLOAT3 m_LifePosition;
	CharacterController* m_Character;
	GameObject* m_Select;
	GameObject* m_Player;
	std::wstring m_Tag;
	SpriteFont* m_pFont = nullptr;
	bool m_IsLeft = true;
	int m_Point = 0;
	bool m_IsFreeze = false;
};

