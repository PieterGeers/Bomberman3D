#pragma once
#include "GameScene.h"

class BomberManInGamePauseMenu final : public GameScene
{
public:
	BomberManInGamePauseMenu();
	~BomberManInGamePauseMenu();

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

