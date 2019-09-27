#include "stdafx.h"
#include "Bom.h"
#include "SpherePrefab.h"
#include "GameScene.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "Scenes/BomberManScene.h"
#include "TilePrefab.h"
#include "ContentManager.h"

std::vector<Bom*> Bom::m_pBombs;
Bom::Bom(bool isRemote)
	:m_IsRemote(isRemote)
{
}

Bom::~Bom()
{}

void Bom::SetBlocksToDestroy(std::vector<GameObject*> destroy)
{
	for (unsigned i = 0; i != destroy.size(); ++i)
	{
		dynamic_cast<TilePrefab*>(destroy[i])->SpawnPowerUp();
		GetScene()->RemoveChild(destroy[i], true);
	}

	GetScene()->RemoveChild(this, true);
}

void Bom::Initialize(const GameContext&)
{
	//Bomb
	if (m_IsRemote)
	{
		auto bomModel = new ModelComponent(L"Resources/Meshes/BomberMan/BomberManRemoteBomb.ovm");
		m_TimeBeforeKaboom = 30.0f;
		bomModel->SetMaterial(10);
		AddComponent(bomModel);
	}
	else
	{
		auto bomModel = new ModelComponent(L"Resources/Meshes/BomberMan/BomberManBomb.ovm");
		bomModel->SetMaterial(4);
		AddComponent(bomModel);
	}
	GetTransform()->Translate(0, 0, 0);
	GetTransform()->Rotate(90, 0, 0);

}

void Bom::PostInitialize(const GameContext&)
{
}

void Bom::Draw(const GameContext&)
{
}

void Bom::PostDraw(const GameContext&)
{
}

void Bom::Update(const GameContext& gameContext)
{
	if (m_HasExploded)
		return;

	m_TimeBeforeKaboom -= gameContext.pGameTime->GetElapsed();
	if (!m_HasExploded && m_TimeBeforeKaboom < 0.0f)
	{
		m_HasExploded = true;
	}
}