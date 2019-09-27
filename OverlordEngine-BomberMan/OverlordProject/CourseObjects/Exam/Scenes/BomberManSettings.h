#pragma once
#include "GameScene.h"

class BomberManSettings final : public GameScene
{
public:
	BomberManSettings();
	~BomberManSettings();

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void SceneActivated() override;
	void SceneDeactivated() override;

private:
	std::vector<GameObject*> m_Buttons = {};
	int m_ActiveButton = 0;
	DirectX::XMFLOAT3 m_PrevScale{ .8f,.8f,.8f };
	std::vector<GameObject*> m_SoundEffectVolumeBar;
	std::vector<GameObject*> m_SoundStreamVolumeBar;
};

