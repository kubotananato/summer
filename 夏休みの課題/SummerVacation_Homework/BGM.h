#pragma once

namespace BgmManager
{
	// 指定したファイルを再生（前のBGMは自動停止）
	void Play(const char* filePath, int volume = 80);

	// 現在のBGMを停止してメモリ解放
	void Stop();
}