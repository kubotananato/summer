#include "GameOverScene.h"
#include "SceneBase.h"
#include <DxLib.h>
#include "TitleScene.h"
#include"GameScene.h"
#include"SceneController.h"
#include"../Application.h"
#include"../FileLoader.h"
#include"../Input.h"

constexpr int fade_interval = 40;

void GameOverScene::InitUpdate(Input&)
{
	auto loader = Application::GetInstance().GetLoader();
	handle_ = loader->LoadGraph(L"Asset/gameover_back.png", AssetType::temporary);

	update_ = &GameOverScene::FadeInUpdate;
	draw_ = &GameOverScene::FadeDraw;
}

void GameOverScene::FadeInUpdate(Input&)
{
	if (--frame_ <= 0)
	{
		update_ = &GameOverScene::NormalUpdate;
		draw_ = &GameOverScene::NormalDraw;
		frame_ = 0;
	}
}

void GameOverScene::NormalUpdate(Input& input)
{

	if (input.IsTriggered("ok"))
	{
		update_ = &GameOverScene::FadeOutUpdate;
		draw_ = &GameOverScene::FadeDraw;
		return;
	}
	if (input.IsTriggered("back"))
	{
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		return;
	}
	frameCount_++;
}

void GameOverScene::FadeOutUpdate(Input&)
{
	if (++frame_ >= fade_interval)
	{
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
}

void GameOverScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	NormalDraw();

	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f* rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameOverScene::NormalDraw()
{
	DrawGraph(0, 0, handle_, true);
	DrawString(0, 32, L"GameOverScene", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", frameCount_);
}

void GameOverScene::EmptyDraw()
{
}

GameOverScene::GameOverScene(SceneController& ctrl) : SceneBase(ctrl),
handle_(-1),
frameCount_(0)
{
	update_ = &GameOverScene::InitUpdate;
	draw_ = &GameOverScene::EmptyDraw;
	frame_ = fade_interval;
}

GameOverScene::~GameOverScene()
{
	Application::GetInstance().GetLoader()->DeleteTemporalAssets();
}

void GameOverScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameOverScene::Draw()
{
	(this->*draw_)();
}
