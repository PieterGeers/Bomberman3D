#pragma once
#include "GameScene.h"
#include "../BomberManStructs.h"

class BomberManGameOver final :public GameScene
{
public:
	BomberManGameOver();
	~BomberManGameOver();

	void SetWinState(WinState state) { m_State = state; }

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneActivated() override;
	void SceneDeactivated() override;

private:
	WinState m_State;
	std::vector<GameObject*> m_Buttons = {};
	int m_ActiveButton = 0;
	DirectX::XMFLOAT3 m_PrevScale{ 1,1,1 };
	GameObject* m_pWinObject = nullptr;
};

