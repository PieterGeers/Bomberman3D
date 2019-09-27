#pragma once
#include "GameObject.h"
#include "SpriteFont.h"

class TimerPrefab final : public GameObject
{
public:
	TimerPrefab(DirectX::XMFLOAT3 position, float time);
	~TimerPrefab();

	void SetPaused(bool paused) { m_Paused = paused; }
	bool GetGameOver() const { return m_GameOver; }
	void ResetGameOver() { m_GameOver = false; }

	void SetTime(float time) { m_Time = time; }
	bool IsPaused() const { return m_Paused; }

protected:
	void Initialize(const GameContext&) override;
	void PostInitialize(const GameContext&) override;
	void Draw(const GameContext&) override;
	void PostDraw(const GameContext&) override;
	void Update(const GameContext&) override;

private:
	SpriteFont* m_pFont = nullptr;
	DirectX::XMFLOAT3 m_Position{};
	float m_Time = 0.0f;
	bool m_Paused = false;
	bool m_GameOver = false;
};

