#pragma once
#include "Vec2.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init();
	void End();
	void Update();
	void Draw();

	// グラフィックハンドルの設定
	void SetGraphHandle(int handle) { m_graphHandle = handle; }


	// 位置情報の取得
	Vec2 GetPos()const { return m_pos; }
	Vec2 GetLastPos()const { return m_lastPos; }

	// 当たり判定用
	Vec2 GetColCenter() const;
	float GetColRadius() const;

	// 登場回数をしゅとくする
//	int GetApperCount() const { return m_apperCount; }

//	void OnHitPlayer() { m_isHit = true; }

private:
	// のこぎりを初期位置に設定する
	void Setup();

private:
	// グラフィックハンドル
	int m_graphHandle;

//	int m_waitFrame;

	// 位置情報
	Vec2 m_pos;
	Vec2 m_lastPos;
	// 移動情報
	Vec2 m_vec;

	int m_hp;
	int m_Attack;


	// 登場回数
//	int m_apperCount;

//	bool m_isHit;

};
