#pragma once

#include "Player.h"
#include "Enemy.h"
#include "Bg.h"
#include "HealPoint.h"
#include "Map.h"

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

	// プレイヤーのポインタを返すゲッター
	Player* GetPlayer() { return &m_player; }

	// 戦闘から復帰した時にフラグを倒す関数
	void ResetFinished();
	// 敵3体をランダム位置に再配置する関数
	void RespawnEnemy();


	bool isGoToBoss = false;

private:
	int m_frameCount;

	// シーンで使用するリソースのハンドル
	int m_playerIdleHandle;
	int m_playerRunHandle;
	int m_enemyIdleHandle;
	int m_enemyRunHandle;
	int m_healPointHandle;

	// 階段用の画像ハンドルと位置
	int m_stairsHandle;
	int m_stairsTileX;
	int m_stairsTileY;

	// ★敵3体の配列管理
	static constexpr int kMaxEnemies = 3;
	Enemy m_enemies[kMaxEnemies];

	bool m_isHit;
	bool m_wasHit;
	bool m_isHealHit;

	// ゲームオーバーになったか?
	bool m_isGameover;

	Player m_player;
	Bg m_bg;
	HealPoint m_healpoint;
	Map m_map;
};

