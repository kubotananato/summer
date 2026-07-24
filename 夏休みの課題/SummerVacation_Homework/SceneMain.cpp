#include "SceneMain.h"
#include "DxLib.h"

SceneMain::SceneMain():
	m_frameCount(0),
	m_playerIdleHandle(-1),
	m_playerRunHandle(-1),
	m_enemyHandle(-1),
	m_isHit(false),
	m_isGameover(false)
{
}

SceneMain::~SceneMain()
{	
}

void SceneMain::Init()
{
	m_playerIdleHandle = LoadGraph("data/Player/PIdle.png");
	m_playerRunHandle = LoadGraph("data/Player/PRun.png");
	m_enemyHandle = LoadGraph("data/Enemy/EIdle.png");

	m_player.SetIdleHandle(m_playerIdleHandle);
	m_player.SetRunHandle(m_playerRunHandle);
	m_enemy.SetEIdleHandle(m_enemyHandle);

	m_frameCount = 0;
	m_player.Init();
	m_enemy.Init();
	m_bg.Init();
}

void SceneMain::End()
{
	//シーンで使用したリソースをメモリから削除
	DeleteGraph(m_playerIdleHandle);
	DeleteGraph(m_playerRunHandle);
	DeleteGraph(m_enemyHandle);

	m_player.End();
	m_enemy.End();
	m_bg.End();
}

void SceneMain::Update()
{
	m_frameCount++;

	m_player.Update();
	m_enemy.Update();
	m_bg.Update();

	Vec2 diff = m_player.GetColCenter() - m_enemy.GetColCenter();

	float radiusSum = m_player.GetRadius() + m_enemy.GetRadius();

	m_isHit = (diff.SqLength() <= radiusSum * radiusSum);
}

void SceneMain::Draw()
{
	m_bg.Draw();
	m_enemy.Draw();
	m_player.Draw();

	// 当たっている時だけ文字を表示
	if (m_isHit)
	{
		DrawString(0, 50, "あたった！", GetColor(255, 0, 0));
	}

	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME%d", m_frameCount);
}
