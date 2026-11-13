#include "GameScene.h"
#include <DxLib.h>
#include <cassert>
#include <cmath>
#include"../Application.h"
#include"../FileLoader.h"
#include"../Input.h"
#include"SceneController.h"
#include"TitleScene.h"
#include"GameOverScene.h"

#include "../Game/Player.h"

constexpr int fade_interval = 20;

void GameScene::InitUpdate(Input&)
{
	player_ = std::make_shared<Player>();

	auto loader = Application::GetInstance().GetLoader();
	handle_ = loader->LoadGraph(L"Asset/game_back.png", AssetType::temporary);

	update_ = &GameScene::FadeInUpdate;
	draw_ = &GameScene::FadeDraw;
}

void GameScene::FadeInUpdate(Input&)
{
	if (--frame_ <= 0)
	{
		update_ = &GameScene::NormalUpdate;
		draw_ = &GameScene::NormalDraw;
		frame_ = 0;
	}
}

void GameScene::NormalUpdate(Input& input)
{
	player_->Update(input);

	if (input.IsTriggered("ok"))
	{
		update_ = &GameScene::FadeOutUpdate;
		draw_ = &GameScene::FadeDraw;
		frame_ = 0;
		return;
	}
	if (input.IsTriggered("back"))
	{
		controller_.ChangeScene(std::make_shared<TitleScene>(controller_));
		return;
	}
	frameCount_++;
}

void GameScene::FadeOutUpdate(Input&)
{
	if (++frame_ >= fade_interval)
	{
		controller_.ChangeScene(std::make_shared<GameOverScene>(controller_));
		return;
	}
}

void GameScene::NormalDraw()
{
	DrawGraph(0, 0, handle_, true);
	DrawString(0, 32, L"GameScene",0xffffff);
	DrawFormatString(0, 16, 0xffffff, L"FRAME:%d", frameCount_);
	player_->Draw();
}

void GameScene::FadeDraw()
{
	const auto& wsize = Application::GetInstance().GetWindowSize();
	NormalDraw();

	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * rate));
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void GameScene::EmptyDraw()
{
}


GameScene::GameScene(SceneController& ctrl) : SceneBase(ctrl),
	handle_(-1),
	frameCount_(0)
{
	update_ = &GameScene::InitUpdate;
	draw_ = &GameScene::EmptyDraw;
	frame_ = fade_interval;
}

GameScene::~GameScene()
{
	Application::GetInstance().GetLoader()->DeleteTemporalAssets();
}

void GameScene::Update(Input& input)
{
	(this->*update_)(input);
}

void GameScene::Draw()
{
	(this->*draw_)();
}



