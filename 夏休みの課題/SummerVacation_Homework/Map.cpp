#include "Map.h"
#include "DxLib.h"
#include <fstream>
#include <sstream>
#include <string>

Map::Map() :
	m_floorHandle(-1),
	m_wallHandle(-1)
{
	// 配列の初期化（すべて 0 にしておく）
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			m_mapData[y][x] = 0;
		}
	}
}

Map::~Map()
{
}

void Map::Init()
{
	// 画像ファイルを読み込む
	m_floorHandle = LoadGraph("data/HealPoint/Map/floor.png");
	m_wallHandle = LoadGraph("data/Map/wall.png");

	// CSVファイルを読み込む
	LoadCSV("data/Map/map.csv");
}

void Map::End()
{
	// メモリから画像を削除する
	DeleteGraph(m_floorHandle);
	DeleteGraph(m_wallHandle);
}

void Map::LoadCSV(const char* filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) return;

	std::string line;
	int y = 0;

	while (std::getline(file, line) && y < MAP_HEIGHT)
	{
		std::stringstream ss(line);
		std::string value;
		int x = 0;

		while (std::getline(ss, value, ',') && x < MAP_WIDTH)
		{
			m_mapData[y][x] = std::stoi(value);
			x++;
		}
		y++;
	}
}

void Map::Draw()
{
	// 2重ループで敷き詰めて描画する
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			// 描画する位置（ピクセル座標）を計算
			int drawX = x * CHIP_SIZE;
			int drawY = y * CHIP_SIZE;

			// 数字に合わせて画像を切り替える
			if (m_mapData[y][x] == 1)
			{
				// 1 のときは壁を描画
				DrawGraph(drawX, drawY, m_wallHandle, TRUE);
			}
			else
			{
				// 0 のときは床を描画
				DrawGraph(drawX, drawY, m_floorHandle, TRUE);
			}
		}
	}
}
