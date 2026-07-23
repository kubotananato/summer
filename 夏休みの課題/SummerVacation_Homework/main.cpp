#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"


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
	SceneMain scene;
	scene.Init();

	while (ProcessMessage() == 0)
	{
		// 現在のフレーム開始時間時刻を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 画面をクリア
		ClearDrawScreen();

		// ゲームの処理
		scene.Update();

		scene.Draw();


		// 画面の書き換え
		ScreenFlip();

		// escキーを押したらゲームを強制終了
		if(CheckHitKey(KEY_INPUT_ESCAPE)) break;

		// 画面のリフレッシュレートにかかわらず1/60経過するまで待つ
		while(GetNowHiPerformanceCount() - start < 16667);
		{
			// 時間経過まで何もせずに待つ
		}

	}

	scene.End();

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;				// ソフトの終了 
}
