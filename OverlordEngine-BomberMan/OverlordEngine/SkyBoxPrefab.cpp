#include "stdafx.h"
#include "SkyBoxPrefab.h"
#include "ModelComponent.h"


SkyBoxPrefab::SkyBoxPrefab(const std::wstring cubeMap)
{
	m_SkyBoxMat = new SkyBoxMaterial();
	m_SkyBoxMat->SetSkyBoxMaterial(L"./Resources/Textures/SkyBox.dds");
}

void SkyBoxPrefab::Initialize(const GameContext& gameContext)
{
	gameContext.pMaterialManager->AddMaterial(m_SkyBoxMat, 5);
	auto skyCube = new ModelComponent(L"./Resources/Meshes/Box.ovm");
	skyCube->SetMaterial(5);
	AddComponent(skyCube);
}
