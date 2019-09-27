#include "stdafx.h"
#include "CharacterController.h"
#include "PhysxManager.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Pmath.h"
#include "ModelComponent.h"
#include "ModelAnimator.h"
#include "ContentManager.h"

CharacterController::CharacterController(float radius, float height, GamepadIndex playerIdx)
	: m_Radius(radius)
	, m_Height(height)
	, m_MaxRunVelocity(60.0f)
	, m_RunAccelerationTime(0.3f)
	, m_RunAcceleration(m_MaxRunVelocity / m_RunAccelerationTime)
	, m_RunVelocity(0)
	, m_PlayerIndex(playerIdx)
	, m_Velocity(0, 0, 0)
{
	m_ResetVelocityValue = m_MaxRunVelocity;
}

void CharacterController::GiveSlownessForTime(float time)
{
	m_MaxRunVelocity = m_SlowMaxRunVelocity;
	m_Slowed = true;
	m_SlowTime = time;
}

void CharacterController::ResetPowerUps()
{
	m_Lives = 0;
	m_Range = 0;
	m_Power = 0;
	m_Ultimate = 0;
	m_Remote = 0;
	m_SlowTime = 0.0f;
	m_Slowed = false;
	m_MaxRunVelocity = m_ResetVelocityValue;
}

void CharacterController::Initialize(const GameContext&)
{
	AddComponent(new ControllerComponent(PhysxManager::GetInstance()->GetPhysics()->createMaterial(0, 0, 0), m_Radius, m_Height));
}

void CharacterController::PostInitialize(const GameContext&)
{
}

void CharacterController::PostDraw(const GameContext&)
{
}

void CharacterController::Update(const GameContext& gameContext)
{
	using namespace DirectX;
	if (m_Freeze)
		return;

	//SLOWNESS
	//********
	if (m_Slowed)
	{
		m_SlowTime -= gameContext.pGameTime->GetElapsed();
		if (m_SlowTime <= 0.0f)
		{
			m_Slowed = false;
			m_MaxRunVelocity = m_ResetVelocityValue;
		}
	}

	//CHARACTER ROTATION
	//******************
	auto look = XMFLOAT2(0, 0);

	look = InputManager::GetThumbstickPosition(true, m_PlayerIndex);
	PVector2 r2{ 1,0 };
	PVector2 l{ look.x, look.y };
	float angle = r2.AngleBetweenInRad(l);
	if (look.y > 0.0f){angle = -angle;}
	if (angle != 0.0f)	m_PreviousAngle = angle;
	else if (angle == 0.0f)	angle = m_PreviousAngle;
	if (look.x > 0.0f && look.y == 0.0f) { angle = 0; }

	GetTransform()->Rotate(0, -1.57f + angle, 0, false);

	//CHARACTER MOVEMENT
	//******************
	XMFLOAT3 f{ 0,0,1 };
	XMVECTOR forward = XMLoadFloat3(&f);
	XMFLOAT3 r{ 1,0,0 };
	XMVECTOR right = XMLoadFloat3(&r);

	const auto direction = (forward * look.y) + (right * look.x);
	if (look.x != 0 || look.y != 0)
	{
		if (m_CurrentAnimation == idle)
		{
			m_CurrentAnimation = run;
			GetComponent<ModelComponent>(true)->GetAnimator()->SetAnimation(m_CurrentAnimation);
		}
		m_RunVelocity += m_RunAcceleration * gameContext.pGameTime->GetElapsed();
		if (m_RunVelocity > m_MaxRunVelocity)
			m_RunVelocity = m_MaxRunVelocity;
		XMStoreFloat3(&m_Velocity, direction*m_RunVelocity);
	}
	else
	{
		if (m_CurrentAnimation == run)
		{
			m_CurrentAnimation = idle;
			GetComponent<ModelComponent>(true)->GetAnimator()->SetAnimation(m_CurrentAnimation);
		}
		m_Velocity.x = 0;
		m_Velocity.z = 0;
	}
	m_Velocity.y = 0;

	XMFLOAT3 temp;
	XMStoreFloat3(&temp, XMLoadFloat3(&m_Velocity) * gameContext.pGameTime->GetElapsed());
	GetComponent<ControllerComponent>()->Move(temp);
}

bool CharacterController::UpdatePowerUps()
{
	switch (m_ActiveBomPowerUp)
	{
	case none: break;
	case range:
		if (!HasRange())
		{m_ActiveBomPowerUp = none;	return true;}
		break;
	case power: 
		if (!HasPower())
		{m_ActiveBomPowerUp = none; return true;}
		break;
	case ultimate:
		if (!HasUltimate())
		{m_ActiveBomPowerUp = none; return true;}
		break;
	case remote:
		if (!HasRemote())
		{m_ActiveBomPowerUp = none; return true;}
		break;
	case live: break;
	case slow: break;
	default: ;
	}
	return false;
}

void CharacterController::Draw(const GameContext&)
{}
