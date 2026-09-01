#include "SceneMain.h"
#include "Game.h"
#include "DxLib.h"
#include <cstdlib>
#include "BGM.h"

SceneMain::SceneMain() :
	m_frameCount(0),
	m_playerIdleHandle(-1),
	m_playerRunHandle(-1),
	m_enemyIdleHandle(-1),
	m_enemyRunHandle(-1),
	m_healPointHandle(-1),
	m_stairsHandle(-1),
	m_stairsTileX(0),
	m_stairsTileY(0),
	m_isHit(false),
	m_wasHit(false),
	m_isHealHit(false),
	m_isGameover(false)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	isFinished = false; // フラグを確実に初期化
	isGoToBoss = false;
	BgmManager::Play("data/BGM/map.mp3", 100);
	m_map.Init();

	// リソースの読み込み
	m_playerIdleHandle = LoadGraph("data/Player/PIdle.png");
	m_playerRunHandle = LoadGraph("data/Player/Walk.png");
	m_enemyIdleHandle = LoadGraph("data/Enemy/EIdle.png");
	m_enemyRunHandle = LoadGraph("data/Enemy/ERun.png"); 
	m_healPointHandle = LoadGraph("data/HealPoint/HealPoint.png");

	m_player.SetIdleHandle(m_playerIdleHandle);
	m_player.SetRunHandle(m_playerRunHandle);
	m_healpoint.SetHealPointHandle(m_healPointHandle);

	m_frameCount = 0;
	m_player.Init();
	m_bg.Init();
	m_healpoint.Init();

	// 敵3体の初期化と画像セット
	for (int i = 0; i < kMaxEnemies; i++)
	{
		m_enemies[i].Init();
		m_enemies[i].SetEIdleHandle(m_enemyIdleHandle);
		m_enemies[i].SetERunHandle(m_enemyRunHandle);
	}

	RespawnEnemy();
}

void SceneMain::End()
{

	BgmManager::Stop();
	isFinished = false;

	m_map.End();

	// シーンで使用したリソースをメモリから削除
	DeleteGraph(m_playerIdleHandle);
	DeleteGraph(m_playerRunHandle);
	DeleteGraph(m_enemyIdleHandle);
	DeleteGraph(m_enemyRunHandle);
	DeleteGraph(m_healPointHandle);

	m_player.End();

	for (int i = 0; i < kMaxEnemies; i++)
	{
		m_enemies[i].End();
	}

	m_bg.End();
	m_healpoint.End();
}

void SceneMain::Update()
{
	m_frameCount++;

	m_player.Update(m_map);
	m_bg.Update();
	m_healpoint.Update(m_player);

	// 敵3体の更新 & プレイヤーとの当たり判定
	m_isHit = false;

	for (int i = 0; i < kMaxEnemies; i++)
	{
		m_enemies[i].Update(m_player.GetPos(), m_map);

		Vec2 diff = m_player.GetColCenter() - m_enemies[i].GetColCenter();
		float radiusSum = m_player.GetColRadius() + m_enemies[i].GetColRadius();

		if (diff.SqLength() <= radiusSum * radiusSum)
		{
			m_isHit = true;
		}
	}

	// 敵と接触した瞬間に通常戦闘へ遷移
	if (m_isHit && !m_wasHit)
	{
		isFinished = true;
	}
	m_wasHit = m_isHit;

	// 回復ポイントとの当たり判定
	{
		Vec2 diff = m_player.GetColCenter() - m_healpoint.GetColCenter();
		float radiusSum = m_player.GetColRadius() + m_healpoint.GetColRadius();

		m_isHealHit = (diff.SqLength() <= radiusSum * radiusSum);
	}

	int playerTileX = static_cast<int>(m_player.GetColCenter().x / CHIP_SIZE);
	int playerTileY = static_cast<int>(m_player.GetColCenter().y / CHIP_SIZE);

	if (m_map.IsStairs(playerTileX, playerTileY))
	{
		isGoToBoss = true;
	}

	// デバッグ機能：Bキーでボス戦へ直行
	if (CheckHitKey(KEY_INPUT_B))
	{
		isGoToBoss = true;
	}
}

void SceneMain::Draw()
{
	m_map.Draw();
	m_bg.Draw();

	// 敵3体をすべて描画
	for (int i = 0; i < kMaxEnemies; i++)
	{
		m_enemies[i].Draw();
	}

	m_player.Draw();
	m_healpoint.Draw();

	// 当たっている時だけ文字を表示
	if (m_isHit)
	{
		DrawString(0, 50, "あたった！", GetColor(255, 0, 0));
	}

	if (m_isHealHit)
	{
		DrawString(0, 60, "回復", GetColor(255, 255, 255));
	}

	DrawFormatString(0, 0, GetColor(255, 255, 255), "HP: %d / %d", m_player.GetHp(), m_player.GetMaxHp());
	DrawFormatString(0, 16, GetColor(255, 255, 255), "MP: %d / %d", m_player.GetMp(), m_player.GetMaxMp());

//	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
//	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME%d", m_frameCount);
}

void SceneMain::ResetFinished()
{
	isFinished = false;

	// 戦闘復帰時に敵3体を再配置
	RespawnEnemy();
}

void SceneMain::RespawnEnemy()
{
	constexpr int kMaxTileX = Game::kScreenWidth / CHIP_SIZE;
	constexpr int kMaxTileY = Game::kScreenHeight / CHIP_SIZE;

	for (int i = 0; i < kMaxEnemies; i++)
	{
		int attempts = 0;
		while (attempts < 100)
		{
			attempts++;

			int tileX = std::rand() % kMaxTileX;
			int tileY = std::rand() % kMaxTileY;

			if (m_map.IsWall(tileX, tileY)) continue;

			float pixelX = static_cast<float>(tileX * CHIP_SIZE);
			float pixelY = static_cast<float>(tileY * CHIP_SIZE);
			Vec2 candidatePos(pixelX, pixelY);

			Vec2 playerDiff = candidatePos - m_player.GetPos();
			if (playerDiff.SqLength() < (CHIP_SIZE * 3) * (CHIP_SIZE * 3)) // プレイヤーから3マス以上離す
			{
				continue;
			}

			bool isOverlap = false;
			for (int j = 0; j < i; j++)
			{
				Vec2 enemyDiff = candidatePos - m_enemies[j].GetPos();
				if (enemyDiff.SqLength() < (CHIP_SIZE * 2) * (CHIP_SIZE * 2)) // 他の敵から2マス以上離す
				{
					isOverlap = true;
					break;
				}
			}

			if (!isOverlap)
			{
				m_enemies[i].SetPos(candidatePos);
				break;
			}
		}
	}
}