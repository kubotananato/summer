#include "TitleScene.h"
#include "SceneController.h"
#include "../FileLoader.h"
#include "../Application.h"
#include "../Input.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include <DxLib.h>

constexpr int fade_interval = 30;

void TitleScene::InitUpdate(Input&)
{
	auto loader = Application::GetInstance().GetLoader();
	backH_ = loader->LoadGraph(L"Asset/title_back.png", AssetType::temporary);

	update_ = &TitleScene::FadeInUpdate;
	draw_ = &TitleScene::FadeDraw;
}

void TitleScene::FadeInUpdate(Input&)
{
	if (--frame_ <= 0)
	{
		// 通常状態に遷移します
		update_ = &TitleScene::NormalUpdate;
		draw_ = &TitleScene::NormalDraw;
		return;
	}
}

void TitleScene::NormalUpdate(Input& input)
{
	if (input.IsTriggered("ok"))
	{
		update_ = &TitleScene::FadeOutUpdate;
		draw_ = &TitleScene::FadeDraw;
		return;
	}
	m_frameCount++;
}

void TitleScene::FadeOutUpdate(Input&)
{
	if (++frame_ >= fade_interval)
	{
		controller_.ChangeScene(std::make_shared<GameScene>(controller_));
		return;
	}
}

void TitleScene::FadeDraw()
{
	// 画面のサイズを取得
	const auto& wsize = Application::GetInstance().GetWindowSize();

	// まず通常の画面を描く
	NormalDraw();

	// フェードの進み具合を0.0～1.0の割合にする
	float rate = static_cast<float>(frame_) / static_cast<float>(fade_interval);

	// 透明度(0〜255)に変換して、黒い幕を半透明にする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, static_cast<int>(255.0f * rate));
	
	// 黒色の四角形を画面全体に描画
	DrawBox(0, 0, wsize.w, wsize.h, 0x000000, true);

	// 半透明の設定をもとに戻す
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void TitleScene::NormalDraw()
{
	DrawGraph(0, 0, backH_, true);
	DrawString(0, 32, L"TitleScene", GetColor(255, 255, 255));
	DrawFormatString(0, 16, GetColor(255, 255, 255), L"FRAME:%d", m_frameCount);
}

void TitleScene::EmptyDraw()
{
}

TitleScene::TitleScene(SceneController& ctrl) : SceneBase(ctrl),
m_frameCount(0),
backH_(-1)
{


	// 最初の状態を代入
	update_ = &TitleScene::InitUpdate;
	draw_ = &TitleScene::EmptyDraw;
	frame_ = fade_interval;
}

TitleScene::~TitleScene()
{
	auto loader = Application::GetInstance().GetLoader();
	loader->DeleteTemporalAssets();
}

void TitleScene::Update(Input& input)
{
	(this->*update_)(input);
}

void TitleScene::Draw()
{
	(this->*draw_)();
}
