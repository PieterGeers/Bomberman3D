#include "stdafx.h"
#include "TilePrefab.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "RigidBodyComponent.h"
#include "ContentManager.h"
#include "ColliderComponent.h"
#include "PhysxManager.h"
#include "PowerUpPrefab.h"
#include "GameScene.h"
#include "Scenes/BomberManScene.h"

TilePrefab::TilePrefab(PVector2 position)
	:m_Position(position)
{
}

TilePrefab::~TilePrefab()
{
}

void TilePrefab::SpawnPowerUp()
{
	int random = rand() % 5;
	if (random == 0)
	{
		auto p = new PowerUpPrefab({ m_Position.x, 5.0f, m_Position.y });
		p->SetTag(L"PowerUp");
		GetScene()->AddChild(p);
		static_cast<BomberManScene*>(GetScene())->AddPickUpToLevel(p);
	}
}

void TilePrefab::Initialize(const GameContext&)
{
	auto tile = new GameObject();
	auto tileModel = new ModelComponent(L"Resources/Meshes/BomberMan/BomberManBlock.ovm");
	tileModel->SetMaterial(5);
	RigidBodyComponent* Rigidbody = new RigidBodyComponent(true);
	std::shared_ptr<physx::PxGeometry> geometry2 = std::make_shared<physx::PxTriangleMeshGeometry>(ContentManager::Load<physx::PxTriangleMesh>(L"Resources/Meshes/BomberMan/BomberManBlock.ovpt"));
	ColliderComponent * collider2 = new ColliderComponent(geometry2, *PhysxManager::GetInstance()->GetPhysics()->createMaterial(0, 0, 0));
	tile->GetTransform()->Translate(m_Position.x, 0, m_Position.y);
	tile->GetTransform()->Rotate(90, 0, 0);
	tile->AddComponent(tileModel);
	tile->AddComponent(Rigidbody);
	tile->AddComponent(collider2);
	AddChild(tile);
}

void TilePrefab::PostInitialize(const GameContext&)
{
}

void TilePrefab::Draw(const GameContext&)
{
}

void TilePrefab::PostDraw(const GameContext&)
{
}

void TilePrefab::Update(const GameContext&)
{
}
