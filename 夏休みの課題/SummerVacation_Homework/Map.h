#pragma once

// マス目とマップ情報の定数定義（1280x720にピッタリ収まる設定）
constexpr int CHIP_SIZE = 40; // 1マスの描画サイズ（ピクセル）
constexpr int MAP_WIDTH = 32; // 横のマス数（1280 / 40 = 32）
constexpr int MAP_HEIGHT = 18; // 縦のマス数（720 / 40 = 18）

class Map
{
public:
	Map();
	~Map();

	void Init();
	void End();
	void Draw();

	// マス目座標 (x, y) が壁かどうか判定する関数
	bool IsWall(int mapX, int mapY) const;

private:
	void LoadCSV(const char* filePath);

	int m_floorHandle;
	int m_wallHandle;
	int m_mapData[MAP_HEIGHT][MAP_WIDTH];
};
