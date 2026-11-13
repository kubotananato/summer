#include <DxLib.h>
#include "Application.h"

// プログラムは WinMain から始まります
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	auto& app = Application::GetInstance();

	if (!app.Init())		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	app.Run(); // メインループ
	app.Terminate(); // 終了処理

	return 0;				// ソフトの終了 
}