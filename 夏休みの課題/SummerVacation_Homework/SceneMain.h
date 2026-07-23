#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Bg.h"

class SceneMain
{
public:
	SceneMain();
	~SceneMain();

	void Init();
	void End();

	void Update();
	void Draw();
	
private:
	int m_frameCount;

	//シーンで使用するリソースのハンドル
	int m_playerIdleHandle;


	// ゲームオーバーになったか?
	bool m_isGameover;

	Player m_player;
	Enemy m_enemy;
	Bg m_bg;
};

