#pragma once
#include "Vec2.h"

class Player;

class HealPoint
{
public:
	HealPoint();
	~HealPoint();

	void Init();
	void End();
	// プレイヤーの参照を受け取るようにする
	void Update(Player& plaeyer);
	void Draw();

	// グラフィックハンドルの設定
	void SetHealPointHandle(int handle) { m_HealHandle = handle; }

	// 位置情報の取得
	Vec2 GetPos()const { return m_pos; }

	// 当たり判定用
	Vec2 GetColCenter() const;
	float GetColRadius() const;

private:

	int m_HealHandle;

	// 位置情報
	Vec2 m_pos;

	//アニメーション管理用変数
	int m_animFrame;

};

