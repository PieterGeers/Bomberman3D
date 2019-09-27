#pragma once
#include "GameScene.h"
#include "../BomberManStructs.h"
#include "../Bom.h"
#include <deque>
#include "../ParticlePrefab.h"
#include "../TimerPrefab.h"
#include "../PowerUpPrefab.h"
#include "../CountDownPrefab.h"
#include "../PowerUpUI.h"

class BomberManScene final : public GameScene
{
public:
	BomberManScene();
	~BomberManScene();

	BomberManScene(const BomberManScene& other) = delete;
	BomberManScene(BomberManScene&& other) = delete;
	BomberManScene& operator=(const BomberManScene& other) = delete;
	BomberManScene& operator=(BomberManScene&& other) noexcept = delete;

	void AddPickUpToLevel(PowerUpPrefab* pickup) { m_pPowerUps.push_back(pickup); }
	void UnfreezePlayers();

	void Restart();

protected:
	void Initialize() override;
	void Update() override;
	void Draw() override;

	void SceneActivated() override;
	void SceneDeactivated() override;

private:
	void SpawnBomb(GameObject* player, PowerUp type);
	void CheckIfDead(int x, int y, int bomRange);
	bool HasExploded(GameObject* player, int x, int y, int range) const;
	
	//BLOCK TO DESTROY BY BOM
	//***********************
	std::vector<GameObject*> GetDestroy(int x, int y, int range, bool explodeAll);

	//BOM PARTICLE
	//************
	void ExplodeBom(int x, int y, int range, bool explodeAll);

	void GameOver();

	PVector2 GetGridIndex(GameObject* object) const;

	CameraComponent* m_pNewCamera = nullptr;
	CameraComponent* m_pDebugCamera = nullptr;
	TimerPrefab* m_pTimer = nullptr;
	CountDownPrefab* m_pCountDown = nullptr;
	std::vector<GameObject*> m_pPlayer;
	bool m_EnableDebugRenderer = false;
	bool m_EnableDebugCamera = false;
	bool m_StartGame = false;
	Grid m_LevelGrid[11][11];
	std::deque<Bom*> m_pBomb;
	Bom* m_pPlayer1RemoteBom = nullptr;
	Bom* m_pPlayer2RemoteBom = nullptr;
	std::deque<ParticlePrefab*> m_pParticles;
	std::vector<PowerUpPrefab*> m_pPowerUps;
	std::vector<PowerUpUI*> m_pUI;
	int m_CurrentRound = 0;
	bool m_HasTutorialPlayed = false;
	float m_TutorialTime = 5.0f;
	GameObject* m_pTutorial = nullptr;
	bool m_Pause = false;

	PostProcessingMaterial* m_pColorBalance;
	bool m_PostProcessingActive = true;
};

