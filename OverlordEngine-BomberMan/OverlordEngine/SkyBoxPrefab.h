#pragma once
#include "GameObject.h"
#include "../OverlordProject/Materials/SkyBoxMaterial.h"

class SkyBoxPrefab final : public GameObject
{
	SkyBoxMaterial* m_SkyBoxMat;
public:
	SkyBoxPrefab(const std::wstring cubeMap);

	SkyBoxPrefab(const SkyBoxPrefab& other) = delete;
	SkyBoxPrefab(SkyBoxPrefab&& other) noexcept = delete;
	SkyBoxPrefab& operator=(const SkyBoxPrefab& other) = delete;
	SkyBoxPrefab& operator=(SkyBoxPrefab&& other) noexcept = delete;
	virtual ~SkyBoxPrefab() = default;

protected:
	void Initialize(const GameContext& gameContext) override;
};

