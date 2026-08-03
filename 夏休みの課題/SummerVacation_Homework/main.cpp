#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "SceneBattle.h"

enum class Scene
{
	Title, // タイトル画面
	Main, // ステージ画面
	Battle // バトル画面
};


// プログラムは WinMain から始まります
int WINAPI WinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow )
{
	ChangeWindowMode(1);

	// ウインドウサイズを16:9に
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	SetMainWindowText("GameTitle");
	if( DxLib_Init() == -1 )		// ＤＸライブラリ初期化処理
	{
		return -1 ;			// エラーが起きたら直ちに終了
	}

	SetDrawScreen(DX_SCREEN_BACK);

	// ゲームシーンの作成
	SceneTitle scenetitle;
	SceneMain scenemain;
	Scenebattle scenebattle;
	
	Scene currentScene = Scene::Title;
	scenetitle.Init();

	while (ProcessMessage() == 0)
	{
		// 現在のフレーム開始時間時刻を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 画面をクリア
		ClearDrawScreen();

		// ゲームの処理

		switch (currentScene)
		{
		case Scene::Title:
			scenetitle.Update();
			if (scenetitle.isFinished)
			{
				scenetitle.End();
				scenemain.Init();
				currentScene = Scene::Main;
			}
			else // ★ else を追加（遷移したフレームは描画しない）
			{
				scenetitle.Draw();
			}
			break;

		case Scene::Main:
			scenemain.Update();
			if (scenemain.isFinished)
			{
				scenemain.End();
				scenebattle.Init();
				currentScene = Scene::Battle;
			}
			else // ★ else を追加
			{
				scenemain.Draw();
			}
			break;

		case Scene::Battle:
			scenebattle.Update();
			if (scenebattle.isFinished)
			{
				scenebattle.End();
				scenemain.Init(); // バトルが終わったらメイン画面に戻る
				currentScene = Scene::Main;
			}
			else // ★ else を追加
			{
				scenebattle.Draw();
			}
			break;
		} 


		// 画面の書き換え
		ScreenFlip();

		// escキーを押したらゲームを強制終了
		if(CheckHitKey(KEY_INPUT_ESCAPE)) break;

		// 画面のリフレッシュレートにかかわらず1/60経過するまで待つ
		while(GetNowHiPerformanceCount() - start < 16667);
		{
			Sleep(1); 
		}

	}

	scenetitle.End();
	scenemain.End();
	scenebattle.End();

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了 
}
