#include "SceneClear.h"
#include "DxLib.h"
#include "BGM.h"
#include "Game.h"

SceneClear::SceneClear() :
	isFinished(false),
	m_oldPad(0),
	m_oldSpace(false),
	m_fontHandle(-1)
{
}

SceneClear::~SceneClear()
{
}

void SceneClear::Init()
{
	isFinished = false;
	BgmManager::Play("data/BGM/kuria.mp3", 100);
	m_fontHandle = CreateFontToHandle(NULL, 64, 3, DX_FONTTYPE_ANTIALIASING);
	m_oldPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	m_oldSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z);
}

void SceneClear::Update()
{
	int currentPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	bool currentSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z);

	// 押した「瞬間」だけ検出（トリガー判定）
	int pushPad = currentPad & ~m_oldPad;
	bool pushSpace = currentSpace && !m_oldSpace;

	m_oldPad = currentPad;
	m_oldSpace = currentSpace;

	if ((pushPad & PAD_INPUT_1) || pushSpace)
	{
		isFinished = true;
	}
}

void SceneClear::End()
{
	BgmManager::Stop();
	if (m_fontHandle != -1)
	{
		DeleteFontToHandle(m_fontHandle);
		m_fontHandle = -1;
	}
}

void SceneClear::Draw()
{
	// 背景をきれいな紺色や黒で塗りつぶす
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(10, 20, 40), TRUE);

	if (m_fontHandle != -1)
	{
		DrawStringToHandle(420, 200, "GAME CLEAR", GetColor(255, 220, 50), m_fontHandle);
	}

	// 達成感を称えるメッセージと操作案内
	DrawString(470, 320, "おめでとう!", GetColor(255, 255, 255));
	DrawString(430, 420, "PRESS BUTTON TO TITLE", GetColor(200, 200, 200));
}
