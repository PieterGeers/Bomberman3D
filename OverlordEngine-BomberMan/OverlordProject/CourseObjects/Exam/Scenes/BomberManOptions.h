#pragma once
#include "GameScene.h"
#include "GameObject.h"

class BomberManOptions final : public GameScene
{
public:
	BomberManOptions();
	~BomberManOptions();

	void SetBackToInGame() { m_BackToInGame = true; }

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
	bool m_BackToInGame = false;
	bool m_GoToSetting = false;
};

