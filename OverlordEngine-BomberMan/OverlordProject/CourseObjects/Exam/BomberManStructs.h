#pragma once
#include "Pmath.h"

enum BMInput
{
	Pause_P1,
	Up_P1,
	Down_P1,
	Left_P1,
	Right_P1,
	Bomb_P1,
	RemoteBom_P1,
	Up_P2,
	Down_P2,
	Left_P2,
	Right_P2,
	Bomb_P2,
	RemoteBom_P2,
	UIButtonSelect,
	UIButtonDown,
	UIButtonUp
};

enum Debug
{
	ShowDebugRenderer = -1,
	SetShadowPosition = -2,
	ShowDebugCamera = -3,
	ShowPostProcessing = -4
};

enum AnimationIdx : UINT
{
	idle = 0,
	run = 1,
	victory = 2
};

struct Grid
{
	PVector2 Position;
	bool hasStone = true;
	bool IsPartOfLevel = false;
	GameObject* TilePrefab = nullptr;
};

enum PowerUp
{
	none,
	range,
	power,
	ultimate,
	remote,
	live,
	slow
};

enum WinState
{
	player1,
	player2,
	tie,
};