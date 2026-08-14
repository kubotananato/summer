#include "SceneGameOver.h"
#include "DxLib.h"

SceneGameOver::SceneGameOver() :
	isFinished(false),
	m_oldPad(0),
	m_oldSpace(false)
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
	isFinished = false;
	m_oldPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	m_oldSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z);
}

void SceneGameOver::Update()
{
	int currentPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	bool currentSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z);

	int pushPad = currentPad & ~m_oldPad;
	bool pushSpace = currentSpace && !m_oldSpace;

	m_oldPad = currentPad;
	m_oldSpace = currentSpace;

	if ((pushPad & PAD_INPUT_1) || pushSpace)
	{
		isFinished = true;
	}
}

void SceneGameOver::Draw()
{
	DrawString(250, 200, "=== GAME OVER ===", GetColor(255, 60, 60));
	DrawString(220, 300, "PRESS BUTTON TO RESTART", GetColor(255, 255, 255));
}