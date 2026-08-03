#include "Scenebattle.h"
#include "DxLib.h"

Scenebattle::Scenebattle()
{
}

Scenebattle::~Scenebattle()
{
}

void Scenebattle::Init()
{
	isFinished = false;
}

void Scenebattle::End()
{
}

void Scenebattle::Update()
{
	int padInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// Aボタン(PAD_INPUT_1) または STARTボタン(PAD_INPUT_10) または SPACEキー が押されたら遷移
	if ((padInput & PAD_INPUT_1) || (padInput & PAD_INPUT_10) || CheckHitKey(KEY_INPUT_SPACE))
	{
		isFinished = true;
	}
}

void Scenebattle::Draw()
{
}