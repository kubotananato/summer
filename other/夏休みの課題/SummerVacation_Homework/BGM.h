#pragma once

namespace BgmManager
{
	// 指定したファイルを再生
	void Play(const char* filePath, int volume = 80);
	void Stop();
}