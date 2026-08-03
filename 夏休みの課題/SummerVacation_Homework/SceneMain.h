#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Bg.h"
#include "HealPoint.h"

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void End();

	void Update();
	void Draw();

	bool isFinished = false;
	
private:
	int m_frameCount;

	//シーンで使用するリソースのハンドル
	int m_playerIdleHandle;
	int m_playerRunHandle;
	int m_enemyHandle;
	int m_healPointHandle;

	bool m_isHit;
	bool m_wasHit;
	bool m_isHealHit;

	// ゲームオーバーになったか?
	bool m_isGameover;

	Player m_player;
	Enemy m_enemy;
	Bg m_bg;
	HealPoint m_healpoint;
};

