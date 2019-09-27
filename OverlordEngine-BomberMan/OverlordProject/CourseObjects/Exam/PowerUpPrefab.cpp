#include "stdafx.h"
#include "PowerUpPrefab.h"
#include "TransformComponent.h"
#include "ModelComponent.h"
#include "RigidBodyComponent.h"
#include "ContentManager.h"
#include "PhysxManager.h"
#include "ColliderComponent.h"
#include "PowerUpUI.h"
#include "GameScene.h"
#include "SoundManager.h"

PowerUpPrefab::PowerUpPrefab(DirectX::XMFLOAT3 position)
	:m_Position(position)
{
}


PowerUpPrefab::~PowerUpPrefab()
{
}

void PowerUpPrefab::Initialize(const GameContext&)
{
	//TRIGGER CALLBACK
	//****************
	auto p1Callback = [=](GameObject*, GameObject* other, GameObject::TriggerAction action)
	{
		if (action == GameObject::TriggerAction::ENTER)
		{
			if (other->GetTag() == L"Player1")
			{
				PowerUp temp = static_cast<PowerUp>(rand() % 6 + 1);
				CharacterController* character = static_cast<CharacterController*>(GetScene()->GetGameObjectWithTag(L"Player1"));
				switch (temp)
				{
				case none: break;
				case range: 
					character->AddRange(1);
					break;
				case power: 
					character->AddPower(2);
					break;
				case ultimate: 
					character->AddUltimate(1);
					break;
				case remote: 
					character->AddRemote(3);
					break;
				case live: 
					character->AddLive();
					break;
				case slow: 
					character->GiveSlownessForTime(10.0f);
					break;
				default: ;
				}
				std::cout << "Player1 entered a powerup!" << std::endl;
				m_Picked = true;
				SoundManager::GetInstance()->GetEffect("PickUp")->Play();
			}
			else if (other->GetTag() == L"Player2")
			{
				PowerUp temp = static_cast<PowerUp>(rand() % 6 + 1);
				CharacterController* character = static_cast<CharacterController*>(GetScene()->GetGameObjectWithTag(L"Player2"));
				switch (temp)
				{
				case none: break;
				case range:
					character->AddRange(1);
					break;
				case power:
					character->AddPower(2);
					break;
				case ultimate:
					character->AddUltimate(1);
					break;
				case remote:
					character->AddRemote(3);
					break;
				case live:
					character->AddLive();
					break;
				case slow:
					character->GiveSlownessForTime(10.0f);
					break;
				default:;
				}
				std::cout << "Player2 entered a powerup!" << std::endl;
				m_Picked = true;
				SoundManager::GetInstance()->GetEffect("PickUp")->Play();
			}
		}
	};
	PhysicsCallback fcP1 = p1Callback;


	auto tileModel = new ModelComponent(L"Resources/Meshes/BomberMan/BomberManPickUp.ovm");
	tileModel->SetMaterial(7);
	RigidBodyComponent* Rigidbody = new RigidBodyComponent(true);
	std::shared_ptr<physx::PxGeometry> geometry = std::make_shared<physx::PxConvexMeshGeometry>(ContentManager::Load<physx::PxConvexMesh>(L"Resources/Meshes/BomberMan/BomberManPickUp.ovpc"));
	ColliderComponent * collider = new ColliderComponent(geometry, *PhysxManager::GetInstance()->GetPhysics()->createMaterial(0, 0, 0));
	GetTransform()->Translate(m_Position.x, m_Position.y, m_Position.z - 3.75f);
	GetTransform()->Rotate(0, 180, 0);
	AddComponent(tileModel);
	AddComponent(Rigidbody);
	AddComponent(collider);
	collider->EnableTrigger(true);
	SetOnTriggerCallBack(fcP1);
}

void PowerUpPrefab::PostInitialize(const GameContext&)
{
}

void PowerUpPrefab::Draw(const GameContext&)
{
}

void PowerUpPrefab::PostDraw(const GameContext&)
{
}

void PowerUpPrefab::Update(const GameContext& /*gameContext*/)
{
	//m_Angle += 45 * gameContext.pGameTime->GetElapsed();
	//GetTransform()->Rotate(0, m_Angle, 0);
}
