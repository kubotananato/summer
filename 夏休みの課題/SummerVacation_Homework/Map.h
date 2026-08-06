#pragma once
class Map
{
	public:
		static const int CHIP_SIZE = 32;  // 1マス 32px
		static const int MAP_WIDTH = 40;  // 横 40マス
		static const int MAP_HEIGHT = 23; // 縦 23マス

		Map();
		~Map();

		void Init();
		void End();
		void Draw();

		// CSV読み込み関数
		void LoadCSV(const char* filePath);

	private:
		int m_wallHandle;
		int m_floorHandle;

		// マップデータ（23行 × 40列）
		int m_mapData[MAP_HEIGHT][MAP_WIDTH];
};

