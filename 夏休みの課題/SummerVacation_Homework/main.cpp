#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "Scenebattle.h" 
#include "SceneBoss.h"   // ★1. ボス部屋ヘッダーを追加
#include "SceneClear.h"
#include "SceneGameOver.h"

enum class Scene
{
	Title,      // タイトル画面
	Main,       // ステージ画面
	Boss,       // ★2. ボス部屋画面を追加
	Battle,     // バトル画面
	BossBattle, // ボス戦
	Clear,      // クリア画面
	GameOver    // ゲームオーバー画面
};

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	ChangeWindowMode(1);
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);
	SetMainWindowText("GameTitle");

	if (DxLib_Init() == -1) return -1;

	SetDrawScreen(DX_SCREEN_BACK);

	// ゲームシーンの作成
	SceneTitle scenetitle;
	SceneMain scenemain;
	SceneBoss sceneboss;     // ★3. ボス部屋シーンインスタンスを作成
	Scenebattle scenebattle;
	SceneClear sceneclear;
	SceneGameOver scenegameover;

	Scene currentScene = Scene::Title;
	scenetitle.Init();

	while (ProcessMessage() == 0 && ClearDrawScreen() == 0)
	{
		LONGLONG start = GetNowHiPerformanceCount();
		ClearDrawScreen();

		switch (currentScene)
		{
		case Scene::Title:
			scenetitle.Update();
			scenetitle.Draw();

			if (scenetitle.isFinished)
			{
				scenemain.Init();
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
			// ② 階段に触れて「ボス部屋」へ遷移
			else if (scenemain.isGoToBoss)
			{
				scenemain.isGoToBoss = false;
				sceneboss.Init(scenemain.GetPlayer()); // ★ ボス部屋の初期化
				currentScene = Scene::Boss;
			}
			break;

		case Scene::Boss: // ★4. ボス部屋の処理を追加
			sceneboss.Update();
			sceneboss.Draw();

			// ボスと会話を終えて戦闘開始フラグが立った時
			if (sceneboss.IsGoToBossBattle())
			{
				scenebattle.Init(scenemain.GetPlayer(), true); // ボス戦開始！
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
					scenegameover.Init();
					currentScene = Scene::GameOver;
				}
				else
				{
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
					sceneclear.Init();
					currentScene = Scene::Clear;
				}
				else if (scenebattle.IsLose())
				{
					scenegameover.Init();
					currentScene = Scene::GameOver;
				}
			}
			break;

		case Scene::Clear:
			sceneclear.Update();
			sceneclear.Draw();

			if (sceneclear.isFinished)
			{
				scenetitle.Init();
				currentScene = Scene::Title;
			}
			break;

		case Scene::GameOver:
			scenegameover.Update();
			scenegameover.Draw();

			if (scenegameover.isFinished)
			{
				scenetitle.Init();
				currentScene = Scene::Title;
			}
			break;
		}

		ScreenFlip();

		if (CheckHitKey(KEY_INPUT_ESCAPE)) break;

		while (GetNowHiPerformanceCount() - start < 16667)
		{
			Sleep(1);
		}
	}

	scenetitle.End();
	scenemain.End();
	sceneboss.End(); // ★5. ボス部屋の終了処理
	scenebattle.End();

	DxLib_End();
	return 0;
}