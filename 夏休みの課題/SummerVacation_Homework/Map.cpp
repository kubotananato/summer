#include "Map.h"
#include "DxLib.h"
#include <fstream>
#include <sstream>
#include <string>

Map::Map() :
	m_floorHandle(-1),
	m_wallHandle(-1),
	m_stairsHandle(-1)
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
	Init("data/map.csv");
}


void Map::Init(const char* filePath)
{
	// 画像ファイルを読み込む
	if (m_floorHandle == -1)
	{
		m_floorHandle = LoadGraph("data/Bg/floor.png");
	}
	if (m_wallHandle == -1)
	{
		m_wallHandle = LoadGraph("data/HealPoint/Map/wall.png");
	}
	if (m_stairsHandle == -1)
	{
		m_stairsHandle = LoadGraph("data/Bg/stairs.png");
	}

	if (m_floorHandle == -1)
	{
		OutputDebugStringA("床画像の読み込みに失敗しました\n");
	}
	if (m_wallHandle == -1)
	{
		OutputDebugStringA("壁画像の読み込みに失敗しました\n");
	}
	if (m_stairsHandle == -1)
	{
		OutputDebugStringA("階段画像の読み込みに失敗しました\n");
	}

	LoadCSV(filePath);
}

void Map::End()
{
	// メモリから画像を削除する
	DeleteGraph(m_floorHandle);
	DeleteGraph(m_wallHandle);
	if (m_stairsHandle != -1)
	{
		DeleteGraph(m_stairsHandle);
		m_stairsHandle = -1;
	}
}

void Map::LoadCSV(const char* filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		return;
	}

	std::string line;
	int y = 0;

	while (std::getline(file, line) && y < MAP_HEIGHT)
	{
		if (y == 0 && line.size() >= 3)
		{
			if (static_cast<unsigned char>(line[0]) == 0xEF &&
				static_cast<unsigned char>(line[1]) == 0xBB &&
				static_cast<unsigned char>(line[2]) == 0xBF)
			{
				line = line.substr(3); // 先頭の3バイトを削る
			}
		}

		if (!line.empty() && line.back() == '\r')
		{
			line.pop_back();
		}

		if (line.empty()) continue;

		std::stringstream ss(line);
		std::string value;
		int x = 0;

		while (std::getline(ss, value, ',') && x < MAP_WIDTH)
		{
			try
			{
				if (!value.empty())
				{
					m_mapData[y][x] = std::stoi(value);
				}
				else
				{
					m_mapData[y][x] = 0;
				}
			}
			catch (const std::exception&)
			{
				m_mapData[y][x] = 0;
			}
			x++;
		}
		y++;
	}
	OutputDebugStringA("★CSVファイルの読み込みに成功しました！\n");
}

void Map::Draw()
{
	// 2重ループで敷き詰めて描画する
	for (int y = 0; y < MAP_HEIGHT; y++)
	{
		for (int x = 0; x < MAP_WIDTH; x++)
		{
			// 1マス（40x40）ごとの描画領域を計算
			int drawX1 = x * CHIP_SIZE;
			int drawY1 = y * CHIP_SIZE;
			int drawX2 = drawX1 + CHIP_SIZE;
			int drawY2 = drawY1 + CHIP_SIZE;

			if (m_mapData[y][x] == 1) // 壁
			{
				SetDrawMode(DX_DRAWMODE_NEAREST);
				SetDrawBright(180, 180, 180);
				DrawExtendGraph(drawX1, drawY1, drawX2 + 1, drawY2 + 1, m_wallHandle, TRUE);
				SetDrawBright(255, 255, 255);
				SetDrawMode(DX_DRAWMODE_BILINEAR);
			}
			else if (m_mapData[y][x] == 2) // 階段マス
			{
				SetDrawMode(DX_DRAWMODE_NEAREST);
				// 下地に床を描画
				DrawExtendGraph(drawX1, drawY1, drawX2 + 1, drawY2 + 1, m_floorHandle, TRUE);

				// 床の上に階段画像を重ねて描画
				if (m_stairsHandle != -1)
				{
					DrawExtendGraph(drawX1, drawY1, drawX2, drawY2, m_stairsHandle, TRUE);
				}
				SetDrawMode(DX_DRAWMODE_BILINEAR);
			}
			else // 床 (0)
			{
				SetDrawMode(DX_DRAWMODE_NEAREST);
				DrawExtendGraph(drawX1, drawY1, drawX2 + 1, drawY2 + 1, m_floorHandle, TRUE);
				SetDrawMode(DX_DRAWMODE_BILINEAR);
			}
		}
	}
}

bool Map::IsWall(int mapX, int mapY) const
{
	// 画面外に出てしまう場合は壁扱いにして外に出られないようにする
	if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
	{
		return true;
	}

	// 1 だったら壁（true）、それ以外なら床（false）を返す
	return m_mapData[mapY][mapX] == 1;
}

bool Map::IsStairs(int mapX, int mapY) const
{
	if (mapX < 0 || mapX >= MAP_WIDTH || mapY < 0 || mapY >= MAP_HEIGHT)
	{
		return false;
	}

	// 2 だったら階段（true）を返す
	return m_mapData[mapY][mapX] == 2;
}