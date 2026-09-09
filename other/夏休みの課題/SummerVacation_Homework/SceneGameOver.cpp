#include "SceneGameOver.h"
#include "DxLib.h"
#include "BGM.h"
#include "Game.h"

SceneGameOver::SceneGameOver() :
	isFinished(false),
	m_oldPad(0),
	m_oldSpace(false),
	m_fontHandle(-1)
{
}

SceneGameOver::~SceneGameOver()
{
}

void SceneGameOver::Init()
{
	isFinished = false;
	BgmManager::Play("data/BGM/make.mp3", 100);
	m_fontHandle = CreateFontToHandle(NULL, 64, 3, DX_FONTTYPE_ANTIALIASING);
	m_oldPad = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	m_oldSpace = CheckHitKey(KEY_INPUT_SPACE) || CheckHitKey(KEY_INPUT_Z);
}

void SceneGameOver::End()
{
	BgmManager::Stop();
	if (m_fontHandle != -1)
	{
		DeleteFontToHandle(m_fontHandle);
		m_fontHandle = -1;
	}
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
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(0, 0, 0), TRUE);
	if (m_fontHandle != -1)
	{
		DrawStringToHandle(450, 220, "GAME OVER", GetColor(255, 0, 0), m_fontHandle);
	}
	DrawString(450, 350, "PRESS BUTTON TO RESTART", GetColor(200, 200, 200));
}