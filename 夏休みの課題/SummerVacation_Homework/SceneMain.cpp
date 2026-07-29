#include "SceneMain.h"
#include "DxLib.h"

SceneMain::SceneMain():
	m_frameCount(0),
	m_playerIdleHandle(-1),
	m_playerRunHandle(-1),
	m_enemyHandle(-1),
	m_healPointHandle(-1),
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
	m_playerIdleHandle = LoadGraph("data/Player/PIdle.png");
	m_playerRunHandle = LoadGraph("data/Player/Walk.png");
	m_enemyHandle = LoadGraph("data/Enemy/EIdle.png");
	m_healPointHandle = LoadGraph("data/HealPoint/HealPoint.png");

	m_player.SetIdleHandle(m_playerIdleHandle);
	m_player.SetRunHandle(m_playerRunHandle);
	m_enemy.SetEIdleHandle(m_enemyHandle);
	m_healpoint.SetHealPointHandle(m_healPointHandle);

	m_frameCount = 0;
	m_player.Init();
	m_enemy.Init();
	m_bg.Init();
	m_healpoint.Init();
}

void SceneMain::End()
{
	//シーンで使用したリソースをメモリから削除
	DeleteGraph(m_playerIdleHandle);
	DeleteGraph(m_playerRunHandle);
	DeleteGraph(m_enemyHandle);
	DeleteGraph(m_healPointHandle);

	m_player.End();
	m_enemy.End();
	m_bg.End();
	m_healpoint.End();
}

void SceneMain::Update()
{
	m_frameCount++;

	m_player.Update();
	m_enemy.Update();
	m_bg.Update();
	m_healpoint.Update(m_player);

  {
	Vec2 diff = m_player.GetColCenter() - m_enemy.GetColCenter();

	float radiusSum = m_player.GetColRadius() + m_enemy.GetColRadius();

	m_isHit = (diff.SqLength() <= radiusSum * radiusSum);
  }
  if (m_isHit && !m_wasHit)
  {
	  m_player.TakeDamage(10);
  }
  m_wasHit = m_isHit;

  {
	  Vec2 diff = m_player.GetColCenter() - m_healpoint.GetColCenter();

	  float radiusSum = m_player.GetColRadius() + m_healpoint.GetColRadius();

	  m_isHealHit = (diff.SqLength() <= radiusSum * radiusSum);

  }
}

void SceneMain::Draw()
{
	m_bg.Draw();
	m_enemy.Draw();
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

	DrawFormatString(10, 80, GetColor(255, 255, 255), "HP: %d / 100", m_player.GetHp());
	DrawFormatString(10, 100, GetColor(255, 255, 255), "MP: %d / 100", m_player.GetMp());

	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME%d", m_frameCount);
}
