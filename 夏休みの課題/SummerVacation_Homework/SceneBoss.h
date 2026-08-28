#pragma once
#include "Map.h"
#include "Player.h"
#include "Vec2.h"

class SceneBoss
{
public:
	static constexpr int kBossAnimFrames = 4;

	SceneBoss();
	~SceneBoss();

	void Init(Player* pPlayer);
	void End();
	void Update();
	void Draw();

	bool IsGoToBossBattle() const { return m_isGoToBossBattle; }

private:
	Map m_map;
	Player* m_pPlayer;

	int m_bossImgHandle[kBossAnimFrames]; // 配列に変更
	Vec2 m_bossPos;

	bool m_isGoToBossBattle;
	bool m_isTalked;
	int m_bgmHandle;
	int m_animTimer;
};