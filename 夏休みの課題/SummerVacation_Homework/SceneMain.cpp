#include "SceneMain.h"
#include "DxLib.h"
#include <cstdlib>

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
	m_map.Init();

	// ----------------------------------------------------
	// リソースの読み込み
	// ----------------------------------------------------
	m_playerIdleHandle = LoadGraph("data/Player/PIdle.png");
	m_playerRunHandle = LoadGraph("data/Player/Walk.png");
	m_enemyIdleHandle = LoadGraph("data/Enemy/EIdle.png");
	m_enemyRunHandle = LoadGraph("data/Enemy/ERun.png"); // ※必要に応じてパスを変更してください
	m_healPointHandle = LoadGraph("data/HealPoint/HealPoint.png");

	m_player.SetIdleHandle(m_playerIdleHandle);
	m_player.SetRunHandle(m_playerRunHandle);
	m_healpoint.SetHealPointHandle(m_healPointHandle);

	m_frameCount = 0;
	m_player.Init();
	m_bg.Init();
	m_healpoint.Init();

	// ----------------------------------------------------
	// 敵3体の初期化と画像セット
	// ----------------------------------------------------
	for (int i = 0; i < kMaxEnemies; i++)
	{
		m_enemies[i].Init();
		m_enemies[i].SetEIdleHandle(m_enemyIdleHandle);
		m_enemies[i].SetERunHandle(m_enemyRunHandle);
	}

	// 敵3体を安全な床タイル上に分散再配置
	RespawnEnemy();
}

void SceneMain::End()
{
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

	// ----------------------------------------------------
	// 敵3体の更新 & プレイヤーとの当たり判定
	// ----------------------------------------------------
	m_isHit = false;

	for (int i = 0; i < kMaxEnemies; i++)
	{
		m_enemies[i].Update();

		// プレイヤーと各敵との距離判定
		Vec2 diff = m_player.GetColCenter() - m_enemies[i].GetColCenter();
		float radiusSum = m_player.GetColRadius() + m_enemies[i].GetColRadius();

		// 3体のうち1体でも当たっていれば Hit とする
		if (diff.SqLength() <= radiusSum * radiusSum)
		{
			m_isHit = true;
		}
	}

	// 敵と接触した瞬間に戦闘シーンへ遷移
	if (m_isHit && !m_wasHit)
	{
		isFinished = true;
	}
	m_wasHit = m_isHit;

	// ----------------------------------------------------
	// 回復ポイントとの当たり判定
	// ----------------------------------------------------
	{
		Vec2 diff = m_player.GetColCenter() - m_healpoint.GetColCenter();
		float radiusSum = m_player.GetColRadius() + m_healpoint.GetColRadius();

		m_isHealHit = (diff.SqLength() <= radiusSum * radiusSum);
	}

	// ----------------------------------------------------
	// 階段（ボス）への到達判定
	// ----------------------------------------------------
	int playerTileX = static_cast<int>((m_player.GetColCenter().x) / CHIP_SIZE);
	int playerTileY = static_cast<int>((m_player.GetColCenter().y) / CHIP_SIZE);

	if (playerTileX == m_stairsTileX && playerTileY == m_stairsTileY)
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

	// ★ 敵3体をすべて描画
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

	DrawFormatString(10, 80, GetColor(255, 255, 255), "HP: %d / %d", m_player.GetHp(), m_player.GetMaxHp());
	DrawFormatString(10, 100, GetColor(255, 255, 255), "MP: %d / %d", m_player.GetMp(), m_player.GetMaxMp());

	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME%d", m_frameCount);
}

void SceneMain::ResetFinished()
{
	isFinished = false;

	// 戦闘復帰時に敵3体を再配置
	RespawnEnemy();
}

void SceneMain::RespawnEnemy()
{
	// 敵3体それぞれに対して安全な（壁でない）配置場所を探して設定する
	for (int i = 0; i < kMaxEnemies; i++)
	{
		while (true)
		{
			int tileX = std::rand() % 32;
			int tileY = std::rand() % 18;

			// 壁判定をチェック
			if (!m_map.IsWall(tileX, tileY))
			{
				float pixelX = static_cast<float>(tileX * CHIP_SIZE);
				float pixelY = static_cast<float>(tileY * CHIP_SIZE);

				m_enemies[i].SetPos(Vec2(pixelX, pixelY));
				break; // 1体分の配置に成功したらループを抜ける
			}
		}
	}
}