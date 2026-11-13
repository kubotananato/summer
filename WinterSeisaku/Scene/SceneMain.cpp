#include "SceneMain.h"
#include "DxLib.h"

SceneMain::SceneMain(SceneController& ctrl) : SceneBase(ctrl),
	m_frameCount(0)
{
}

SceneMain::~SceneMain()
{
}

void SceneMain::Init()
{
	m_frameCount = 0;
}

void SceneMain::End()
{
}

void SceneMain::Update(Input& input)
{
	m_frameCount++;
}


void SceneMain::Draw()
{
	DrawString(0,0,L"SceneMain", GetColor(255,255,255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", m_frameCount);
}
