#pragma once
#include "ControllerComponent.h"
#include "GameObject.h"
#include "BomberManStructs.h"

class CharacterController final : public GameObject
{
public:
	CharacterController(float radius, float height, GamepadIndex playerIdx);
	~CharacterController() = default;

	void Freeze() { m_Freeze = true; }
	void UnFreeze() { m_Freeze = false; }

	void GiveSlownessForTime(float time);
	int GetSlowTime() const { return static_cast<int>(m_SlowTime); }

	void AddLive() { ++m_Lives; }
	void SubLive() { --m_Lives; }
	bool HasLives() const { return m_Lives > 0; }
	int GetLives() const { return m_Lives; }

	void AddRange(int amount) { m_Range += amount; }
	void SubRange() { --m_Range; }
	bool HasRange() const { return m_Range > 0; }
	int GetRange() const { return m_Range; }

	void AddPower(int amount) { m_Power += amount; }
	void SubPower() { --m_Power; }
	bool HasPower() const { return m_Power > 0; }
	int GetPower() const { return m_Power; }

	void AddUltimate(int amount) { m_Ultimate += amount; }
	void SubUltimate() { --m_Ultimate; }
	bool HasUltimate() const { return m_Ultimate > 0; }
	int GetUltimate() const { return m_Ultimate; }

	void AddRemote(int amount) { m_Remote += amount; }
	void SubRemote() { --m_Remote; }
	bool HasRemote() const { return m_Remote > 0; }
	int GetRemote() const { return m_Remote; }

	PowerUp GetActiveBomPowerUp() const { return m_ActiveBomPowerUp; }
	void SetActiveBomPowerUp(PowerUp pu) { m_ActiveBomPowerUp = pu; }
	void ResetPowerUps();
	bool UpdatePowerUps();

protected:
	void Initialize(const GameContext&) override;
	void PostInitialize(const GameContext&) override;
	void Draw(const GameContext&) override;
	void PostDraw(const GameContext&) override;
	void Update(const GameContext&) override;

private:
	bool m_Freeze = false;
	bool m_Slowed = false;
	float m_SlowTime = 0.0f;
	float m_SlowMaxRunVelocity = 30.0f;
	float m_ResetVelocityValue;
	PowerUp m_ActiveBomPowerUp = PowerUp::none;
	
	int m_Lives = 0;
	int m_Range = 0;
	int m_Power = 0;
	int m_Ultimate = 0;
	int m_Remote = 0;

	float m_Radius, m_Height;

	//Running
	float m_MaxRunVelocity,
		m_RunAccelerationTime,
		m_RunAcceleration,
		m_RunVelocity;
	float m_PreviousAngle = 0.0f;
	GamepadIndex m_PlayerIndex;
	DirectX::XMFLOAT3 m_Velocity;

	AnimationIdx m_CurrentAnimation = idle;

};

