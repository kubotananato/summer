#include "Application.h"
#include <DxLib.h>
#include"Input.h"
#include"Scene/SceneController.h"
#include "Scene/TitleScene.h"
#include "FileLoader.h"

// 画面情報
constexpr int kScreenWidth = 1280;
constexpr int kScreenHeight = 720;

std::shared_ptr<FileLoader>Application::GetLoader()
{
	return loader_;
}

Application& Application::GetInstance()
{
	static Application instance;
	return instance;
}

Application::Application()
{
	wsize_ = { kScreenWidth,kScreenHeight };
	loader_ = std::make_shared<FileLoader>();
}

bool Application::Init()
{
	// 初期化
	ChangeWindowMode(true);

	// 画面のサイズ設定
	SetGraphMode(wsize_.w, wsize_.h, 32);

	if (DxLib_Init() == -1)
	{
		return false;
	}

	SetDrawScreen(DX_SCREEN_BACK);
	return true;
}

void Application::Run()
{
	Input input;
	SceneController controller;

	controller.ChangeScene(std::make_shared<TitleScene>(controller));

	while (ProcessMessage() != -1 && !requestExit_)
	{
		// 現在のフレーム開始時刻を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 入力情報の更新
		input.Update();

		// 画面をクリア
		ClearDrawScreen();

		controller.Update(input);

		controller.Draw();

		// 画面の書き換えを待つ
		ScreenFlip();

		// escキーを押したらゲームを強制終了
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;

		// 画面のリフレッシュレートにかかわらず1/60経過するまで待つ
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 時間経過まで何もせずに待つ
		}
	}
}

void Application::Terminate()
{
	DxLib_End();
}

const Size& Application::GetWindowSize() const
{
	return wsize_;
}

void Application::RequestExit()
{
	requestExit_ = true;
}
