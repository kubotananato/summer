#pragma once

// マス目とマップ情報の定数定義
constexpr int CHIP_SIZE = 40;
constexpr int MAP_WIDTH = 32;
constexpr int MAP_HEIGHT = 18;

class Map
{
public:
	Map();
	~Map();

	void Init();
	void Init(const char* filePath);
	void End();
	void Draw();

	// マス目座標 (x, y) が壁かどうか判定する関数
	bool IsWall(int mapX, int mapY) const;

	bool IsStairs(int mapX, int mapY) const;

private:
	void LoadCSV(const char* filePath);

	int m_floorHandle;
	int m_wallHandle;
	int m_stairsHandle;
	int m_mapData[MAP_HEIGHT][MAP_WIDTH];
};
