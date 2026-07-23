#include "SceneMain.h"
#include "DxLib.h"

SceneMain::SceneMain():
	m_frameCount(0),
	m_playerIdleHandle(-1),
	m_isGameover(false)
{
}

SceneMain::~SceneMain()
{	
}

void SceneMain::Init()
{
	m_playerIdleHandle = LoadGraph("data/Player/PIdle.png");

	m_player.SetIdleHandle(m_playerIdleHandle);

	m_frameCount = 0;
	m_player.Init();
	m_enemy.Init();
	m_bg.Init();
}

void SceneMain::End()
{
	//シーンで使用したリソースをメモリから削除
	DeleteGraph(m_playerIdleHandle);

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
}

void SceneMain::Draw()
{
	// まず一番奥に背景を描画
	m_bg.Draw();

	// 次にキャラクター類を描画
	m_enemy.Draw();
	m_player.Draw();

	DrawString(0, 0, "SceneMain", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), "FRAME%d", m_frameCount);
}
