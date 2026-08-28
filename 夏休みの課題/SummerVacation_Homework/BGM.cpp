#include "BGM.h"
#include "DxLib.h"

namespace
{
	int g_currentBgmHandle = -1; // 現在再生中のBGMハンドル
}

namespace BgmManager
{
	void Play(const char* filePath, int volume)
	{
		// すでに流れているBGMがあれば停止して削除する
		Stop();

		// BGM用にストリーミング形式で読み込む設定
		SetCreateSoundDataType(DX_SOUNDDATATYPE_FILE);

		g_currentBgmHandle = LoadSoundMem(filePath);

		// 設定をデフォルトに戻す
		SetCreateSoundDataType(DX_SOUNDDATATYPE_MEMNOPRESS);

		if (g_currentBgmHandle != -1)
		{
			ChangeVolumeSoundMem(volume, g_currentBgmHandle);
			PlaySoundMem(g_currentBgmHandle, DX_PLAYTYPE_LOOP); // ループ再生
		}
	}

	void Stop()
	{
		if (g_currentBgmHandle != -1)
		{
			StopSoundMem(g_currentBgmHandle);
			DeleteSoundMem(g_currentBgmHandle);
			g_currentBgmHandle = -1;
		}
	}
}
