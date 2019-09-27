#pragma once
#include "GameObject.h"

class CountDownPrefab final : public GameObject
{
public:
	CountDownPrefab();
	~CountDownPrefab();

	bool GetCountDownComplete() const { return m_CountDownComplete; }
	void Freeze() { m_IsFreeze = true; }
	void UnFreeze() { m_IsFreeze = false; }

protected:
	void Initialize(const GameContext&) override;
	void PostInitialize(const GameContext&) override;
	void Draw(const GameContext&) override;
	void PostDraw(const GameContext&) override;
	void Update(const GameContext&) override;

private:
	int m_CountDown = 3;
	float m_AccTime = 0.0f;
	bool m_CountDownComplete = false;
	bool m_IsFreeze = false;
};

