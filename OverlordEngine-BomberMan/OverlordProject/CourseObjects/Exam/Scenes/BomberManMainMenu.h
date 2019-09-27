#pragma once
#include "GameScene.h"

class BomberManMainMenu final : public GameScene
{
public:
	BomberManMainMenu();
	~BomberManMainMenu() = default;

	BomberManMainMenu(const BomberManMainMenu& other) = delete;
	BomberManMainMenu(BomberManMainMenu&& other) = delete;
	BomberManMainMenu& operator=(const BomberManMainMenu& other) = delete;
	BomberManMainMenu& operator=(BomberManMainMenu&& other) noexcept = delete;

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void SceneActivated() override;
	void SceneDeactivated() override;

private:
	std::vector<GameObject*> m_Buttons = {};
	int m_ActiveButton = 0;
	DirectX::XMFLOAT3 m_PrevScale{ 1,1,1 };
};

