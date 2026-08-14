#include "SceneTitle.h"
#include "DxLib.h"

SceneTitle::SceneTitle() :
	isFinished(false),
	m_oldPad(0),
	m_oldSpace(false)
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	isFinished = false;

	// ★ 起動時や他画面からのボタン押しっぱなし持ち越しを防ぐ
	m_oldPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	m_oldSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z) || CheckHitKey(KEY_INPUT_RETURN);
}

void SceneTitle::End()
{
}

void SceneTitle::Update()
{
	int currentPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	bool currentSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z) || CheckHitKey(KEY_INPUT_RETURN);

	// ★ 押した「瞬間」だけを検出（トリガー判定）
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
	DrawString(200, 200, "=== TITLE SCREEN (Press SPACE) ===", GetColor(255, 255, 255));
}
