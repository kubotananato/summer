#include "SceneTitle.h"
#include "DxLib.h"
#include "BGM.h"
#include "Game.h"
#include <cstring>

SceneTitle::SceneTitle() :
	isFinished(false),
	m_oldPad(0),
	m_oldSpace(false),
	m_blinkTimer(0)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	isFinished = false;
	BgmManager::Play("data/BGM/Title.mp3", 150);
	m_titleMap.Init("data/Title.csv");

	// 起動時や他画面からのボタン押しっぱなし持ち越しを防ぐ
	m_oldPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	m_oldSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z) || CheckHitKey(KEY_INPUT_RETURN);
	m_blinkTimer = 0;
}

void SceneTitle::End()
{
	BgmManager::Stop();
	m_titleMap.End();
}

void SceneTitle::Update()
{
	m_blinkTimer++;

	int currentPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	bool currentSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z) || CheckHitKey(KEY_INPUT_RETURN);

	// 押した「瞬間」だけを検出（トリガー判定）
	int pushPad = currentPad & ~m_oldPad;
	bool pushSpace = currentSpace && !m_oldSpace;

	m_oldPad = currentPad;
	m_oldSpace = currentSpace;

	// ボタン/キーが「新しく押された瞬間」だけ遷移
	if ((pushPad & PAD_INPUT_1) || pushSpace)
	{
		isFinished = true;
	}
}

void SceneTitle::Draw()
{
	m_titleMap.Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 120);
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
//	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(10, 10, 30), TRUE);

	const char* titleText = "--- 勇者の軌跡 ---";
	int titleWidth = GetDrawStringWidth(titleText, (int)std::strlen(titleText));
	int titleX = (Game::kScreenWidth - titleWidth) / 2;
	DrawString(titleX, 220, titleText, GetColor(255, 220, 50));

	if ((m_blinkTimer / 30) % 2 == 0)
	{
		const char* pushText = "- PRESS SPACE or Z KEY -";
		int pushWidth = GetDrawStringWidth(pushText, (int)std::strlen(pushText));
		int pushX = (Game::kScreenWidth - pushWidth) / 2;
		DrawString(pushX, 420, pushText, GetColor(255, 255, 255));
	}
}
