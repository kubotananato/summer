#include "DxLib.h"
#include "Game.h"
#include "SceneMain.h"
#include "SceneTitle.h"
#include "Scenebattle.h" 
#include "SceneBoss.h"   
#include "SceneClear.h"
#include "SceneGameOver.h"
#include "BGM.h" 

enum class Scene
{
	Title,      // タイトル画面
	Main,       // ステージ画面
	Boss,       // ボス部屋画面
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
	SceneBoss sceneboss;
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
				scenetitle.End(); 
				scenemain.Init(); // メインへ
				currentScene = Scene::Main;
			}
			break;

		case Scene::Main:
			scenemain.Update();
			scenemain.Draw();

			if (scenemain.isFinished)
			{
				BgmManager::Stop();
				scenemain.ResetFinished();
				scenebattle.Init(scenemain.GetPlayer(), false);
				currentScene = Scene::Battle;
			}

			else if (scenemain.isGoToBoss)
			{
				scenemain.isGoToBoss = false;
				sceneboss.Init(scenemain.GetPlayer());
				currentScene = Scene::Boss;
			}
			break;

		case Scene::Boss:
			sceneboss.Update();
			sceneboss.Draw();

	
			if (sceneboss.IsGoToBossBattle())
			{
				sceneboss.End(); 
				scenebattle.Init(scenemain.GetPlayer(), true); 
				currentScene = Scene::BossBattle;
			}
			break;

		case Scene::Battle: // 通常バトル
			scenebattle.Update();
			scenebattle.Draw();

			if (scenebattle.isFinished)
			{
				scenebattle.End(); 

				if (scenebattle.IsLose())
				{
					scenegameover.Init();
					currentScene = Scene::GameOver;
				}
				else
				{
					BgmManager::Play("data/BGM/map.mp3", 100);
					currentScene = Scene::Main;
				}
			}
			break;

		case Scene::BossBattle: // ボスバトル
			scenebattle.Update();
			scenebattle.Draw();

			if (scenebattle.isFinished)
			{
				scenebattle.End();

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
				sceneclear.End();
				scenetitle.Init();
				currentScene = Scene::Title;
			}
			break;

		case Scene::GameOver:
			scenegameover.Update();
			scenegameover.Draw();

			if (scenegameover.isFinished)
			{
				scenegameover.End(); 
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
	sceneboss.End();
	scenebattle.End();
	sceneclear.End();    
	scenegameover.End(); 

	DxLib_End();
	return 0;
}