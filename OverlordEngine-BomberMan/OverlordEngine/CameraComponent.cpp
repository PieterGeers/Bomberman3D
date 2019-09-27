#include "stdafx.h"
#include "CameraComponent.h"
#include "OverlordGame.h"
#include "TransformComponent.h"
#include "PhysxProxy.h"
#include "GameObject.h"
#include "GameScene.h"
#include "RigidBodyComponent.h"


CameraComponent::CameraComponent():
	m_FarPlane(2500.0f),
	m_NearPlane(0.1f),
	m_FOV(DirectX::XM_PIDIV4),
	m_Size(25.0f),
	m_IsActive(true),
	m_PerspectiveProjection(true)
{
	XMStoreFloat4x4(&m_Projection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_View, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewInverse, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjection, DirectX::XMMatrixIdentity());
	XMStoreFloat4x4(&m_ViewProjectionInverse, DirectX::XMMatrixIdentity());

	const auto windowSettings = OverlordGame::GetGameSettings().Window;
	m_HalfHeight = windowSettings.Height / 2.0f;
	m_HalfWidth = windowSettings.Width / 2.0f;
}

void CameraComponent::Initialize(const GameContext&){}

void CameraComponent::Update(const GameContext&)
{
	// see https://stackoverflow.com/questions/21688529/binary-directxxmvector-does-not-define-this-operator-or-a-conversion
	using namespace DirectX;

	const auto windowSettings = OverlordGame::GetGameSettings().Window;
	DirectX::XMMATRIX projection;

	if(m_PerspectiveProjection)
	{
		projection = DirectX::XMMatrixPerspectiveFovLH(m_FOV, windowSettings.AspectRatio ,m_NearPlane, m_FarPlane);
	}
	else
	{
		const float viewWidth = (m_Size>0) ? m_Size * windowSettings.AspectRatio : windowSettings.Width;
		const float viewHeight = (m_Size>0) ? m_Size : windowSettings.Height;
		projection = DirectX::XMMatrixOrthographicLH(viewWidth, viewHeight, m_NearPlane, m_FarPlane);
	}

	const DirectX::XMVECTOR worldPosition = XMLoadFloat3(&GetTransform()->GetWorldPosition());
	const DirectX::XMVECTOR lookAt = XMLoadFloat3(&GetTransform()->GetForward());
	const DirectX::XMVECTOR upVec = XMLoadFloat3(&GetTransform()->GetUp());

	const DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(worldPosition, worldPosition + lookAt, upVec);
	const DirectX::XMMATRIX viewInv = XMMatrixInverse(nullptr, view);
	const DirectX::XMMATRIX viewProjectionInv = XMMatrixInverse(nullptr, view * projection);

	XMStoreFloat4x4(&m_Projection, projection);
	XMStoreFloat4x4(&m_View, view);
	XMStoreFloat4x4(&m_ViewInverse, viewInv);
	XMStoreFloat4x4(&m_ViewProjection, view * projection);
	XMStoreFloat4x4(&m_ViewProjectionInverse, viewProjectionInv);
}

void CameraComponent::Draw(const GameContext&) {}

void CameraComponent::SetActive()
{
	auto gameObject = GetGameObject();
	if(gameObject == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game object is null");
		return;
	}

	auto gameScene = gameObject->GetScene();
	if(gameScene == nullptr)
	{
		Logger::LogError(L"[CameraComponent] Failed to set active camera. Parent game scene is null");
		return;
	}

	gameScene->SetActiveCamera(this);
}

GameObject* CameraComponent::Pick(const GameContext& gameContext, CollisionGroupFlag ignoreGroups) const
{
	using namespace DirectX;
	using namespace physx;
	UNREFERENCED_PARAMETER(gameContext);
	UNREFERENCED_PARAMETER(ignoreGroups);
		
	//TODO implement
	//STEP1 convert mouse coordinates to NDC coords
	float xNdc = (gameContext.pInput->GetMousePosition().x - m_HalfWidth) / m_HalfWidth;
	float yNdc = (m_HalfHeight - gameContext.pInput->GetMousePosition().y) / m_HalfHeight;

	//STEP2 calculate near point and far point 
	XMMATRIX viewProjectionInverse = XMLoadFloat4x4(&m_ViewProjectionInverse);
	
	//NearPoint
	XMFLOAT3 nearPoint = { xNdc,yNdc,0.0f };
	XMVECTOR nearVec = XMLoadFloat3(&nearPoint);
	nearVec = XMVector3TransformCoord(nearVec, viewProjectionInverse);
	DirectX::XMStoreFloat3(&nearPoint, nearVec);
	PxVec3 pxNearPoint{ nearPoint.x, nearPoint.y, nearPoint.z };


	XMFLOAT3 farPoint = { xNdc,yNdc,1.0f };
	XMVECTOR farVec = XMLoadFloat3(&farPoint);
	farVec = XMVector3TransformCoord(farVec, viewProjectionInverse);
	DirectX::XMStoreFloat3(&farPoint, farVec);
	PxVec3 pxFarPoint{ farPoint.x, farPoint.y, farPoint.z };

	//STEP3 raycast
	PxQueryFilterData filterData;
	filterData.data.word0 = ~ignoreGroups;

	PxRaycastBuffer hit;

	auto physXProxy = GetGameObject()->GetScene()->GetPhysxProxy();
	PxVec3 pxDir = pxFarPoint - pxNearPoint;
	
	if (physXProxy->Raycast(pxNearPoint, pxDir.getNormalized(), PX_MAX_F32,hit, PxHitFlag::eDEFAULT, filterData))
	{
		PxRaycastHit raycastHit = hit.getAnyHit(0);
		if (raycastHit.actor->userData)
		{
			return static_cast<RigidBodyComponent*>(raycastHit.actor->userData)->GetGameObject();
		}
	}

	return nullptr;
}