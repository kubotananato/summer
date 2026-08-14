#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "Scenebattle.h" // 小文字の b に統一
#include "SceneClear.h"
#include "SceneGameOver.h"

enum class Scene
{
	Title, // タイトル画面
	Main,  // ステージ画面
	Battle, // バトル画面
	BossBattle, // ボス戦
	Clear,      // クリア画面
	GameOver    // ゲームオーバー画面
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
	SceneClear sceneclear;
	SceneGameOver scenegameover;
	
	Scene currentScene = Scene::Title;
	scenetitle.Init();

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		// 現在のフレーム開始時間時刻を取得
		LONGLONG start = GetNowHiPerformanceCount();

		// 画面をクリア
		ClearDrawScreen();

		// ゲームの処理
		switch (currentScene)
		{
			// ★ ここを追加！
		case Scene::Title:
			scenetitle.Update();
			scenetitle.Draw();

			if (scenetitle.isFinished)
			{
				scenemain.Init(); // メイン画面の初期化
				currentScene = Scene::Main;
			}
			break;

		case Scene::Main:
			scenemain.Update();
			scenemain.Draw();

			// ① 通常エンカウント時の遷移
			if (scenemain.isFinished)
			{
				scenemain.ResetFinished();
				scenebattle.Init(scenemain.GetPlayer(), false); // 通常戦
				currentScene = Scene::Battle;
			}
			// ② 階段に触れてボス戦への遷移
			else if (scenemain.isGoToBoss)
			{
				scenemain.isGoToBoss = false;
				scenebattle.Init(scenemain.GetPlayer(), true); // ボス戦！
				currentScene = Scene::BossBattle;
			}
			break;

		case Scene::Battle: // 通常バトル
			scenebattle.Update();
			scenebattle.Draw();

			if (scenebattle.isFinished)
			{
				if (scenebattle.IsLose())
				{
					// 敗北したらゲームオーバーへ
					scenegameover.Init();
					currentScene = Scene::GameOver;
				}
				else
				{
					// 勝利・逃走時はメインマップへ戻る
					currentScene = Scene::Main;
				}
			}
			break;

		case Scene::BossBattle: // ボスバトル
			scenebattle.Update();
			scenebattle.Draw();

			if (scenebattle.isFinished)
			{
				if (scenebattle.IsWin())
				{
					// ボス撃破でゲームクリア！
					sceneclear.Init();
					currentScene = Scene::Clear;
				}
				else if (scenebattle.IsLose())
				{
					// 敗北したらゲームオーバー
					scenegameover.Init();
					currentScene = Scene::GameOver;
				}
			}
			break;

		case Scene::Clear: // クリア画面
			sceneclear.Update();
			sceneclear.Draw();

			if (sceneclear.isFinished)
			{
				scenetitle.Init(); // ★ タイトル画面を初期化して戻る
				currentScene = Scene::Title;
			}
			break;

		case Scene::GameOver: // ゲームオーバー画面
			scenegameover.Update();
			scenegameover.Draw();

			if (scenegameover.isFinished)
			{
				scenetitle.Init(); // ★ タイトル画面を初期化して戻る
				currentScene = Scene::Title;
			}
			break;
		}

		// 画面の書き換え
		ScreenFlip();

		// escキーを押したらゲームを強制終了
		if(CheckHitKey(KEY_INPUT_ESCAPE)) break;

		// 画面のリフレッシュレートにかかわらず1/60経過するまで待つ
		while (GetNowHiPerformanceCount() - start < 16667)
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