#include "SceneTitle.h"
#include "DxLib.h"

SceneTitle::SceneTitle()
{
}

SceneTitle::~SceneTitle()
{
}

void SceneTitle::Init()
{
	isFinished = false; // ★追加！
}

void SceneTitle::End()
{
}

void SceneTitle::Update()
{
	int padInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	if ((padInput & PAD_INPUT_1) || CheckHitKey(KEY_INPUT_SPACE))
	{
		isFinished = true; // 終了フラグを true に！
	}
}

void SceneTitle::Draw()
{
	DrawString(200, 200, "=== TITLE SCREEN (Press SPACE) ===", GetColor(255, 255, 255));
}
