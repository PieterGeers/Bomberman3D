
#include "stdafx.h"

#include "MainGame.h"
#include "GeneralStructs.h"
#include "SceneManager.h"
#include "PhysxProxy.h"
#include "DebugRenderer.h"
#include "CourseObjects/Exam/SoundStream.h"
#include "SoundManager.h"

#define Game

#ifdef Game
#include "CourseObjects/Exam/Scenes/BomberManScene.h"
#include "CourseObjects/Exam/Scenes/BomberManMainMenu.h"
#include "CourseObjects/Exam/Scenes/BomberManOptions.h"
#include "CourseObjects/Exam/Scenes/BomberManInGamePauseMenu.h"
#include "CourseObjects/Exam/Scenes/BomberManGameOver.h"
#include "CourseObjects/Exam/Scenes/BomberManSettings.h"
#endif


MainGame::MainGame(void)
{}

MainGame::~MainGame(void)
{}

//Game is preparing
void MainGame::OnGamePreparing(GameSettings& gameSettings)
{
	UNREFERENCED_PARAMETER(gameSettings);
	//Nothing to do atm.
}

void MainGame::Initialize()
{

#ifdef Game
	SceneManager::GetInstance()->AddGameScene(new BomberManScene());
	SceneManager::GetInstance()->AddGameScene(new BomberManMainMenu());
	SceneManager::GetInstance()->AddGameScene(new BomberManOptions());
	SceneManager::GetInstance()->AddGameScene(new BomberManInGamePauseMenu());
	SceneManager::GetInstance()->AddGameScene(new BomberManGameOver());
	SceneManager::GetInstance()->AddGameScene(new BomberManSettings());
	SceneManager::GetInstance()->SetActiveGameScene(L"BomberManMainMenu");
	SoundManager::GetInstance()->AddSoundStream(new SoundStream("./Resources/Sounds/Menu.mp3", "Menu"));
	SoundManager::GetInstance()->AddSoundStream(new SoundStream("./Resources/Sounds/Game.mp3", "Game"));
	SoundManager::GetInstance()->AddSoundEffect(new SoundEffect("./Resources/Sounds/PickUp.wav", "PickUp"));
	SoundManager::GetInstance()->AddSoundEffect(new SoundEffect("./Resources/Sounds/Countdown1.wav", "Countdown1"));
	SoundManager::GetInstance()->AddSoundEffect(new SoundEffect("./Resources/Sounds/Countdown2.wav", "Countdown2"));
	SoundManager::GetInstance()->AddSoundEffect(new SoundEffect("./Resources/Sounds/Explosion.wav", "Explosion"));
	DebugRenderer::ToggleDebugRenderer();
#endif

}

LRESULT MainGame::WindowProcedureHook(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	switch (message)
	{
		case WM_KEYUP:
		{
			if ((lParam & 0x80000000) != 0x80000000)
				return -1;

			else if (wParam == VK_F4)
			{
				DebugRenderer::ToggleDebugRenderer();
				return 0;
			}
		}
	}

	return -1;
}
