#include "SceneClear.h"
#include "DxLib.h"

SceneClear::SceneClear() :
	isFinished(false),
	m_oldPad(0),
	m_oldSpace(false)
{
}

SceneClear::~SceneClear()
{
}

void SceneClear::Init()
{
	isFinished = false;
	// バトル等からのボタン押しっぱなし持ち越しを防ぐ
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

void SceneClear::Draw()
{
	DrawString(250, 200, "=== GAME CLEAR ===", GetColor(255, 255, 100));
	DrawString(220, 300, "PRESS BUTTON TO TITLE", GetColor(255, 255, 255));
}
