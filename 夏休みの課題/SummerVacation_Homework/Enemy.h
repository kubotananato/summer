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
	void SetEIdleHandle(int handle) { m_IdleHandle = handle; }
	void SetERunHandle(int handle) { m_RunHandle = handle; }


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
//	void Setup();

private:
	// グラフィックハンドル
	int m_IdleHandle;
	int m_RunHandle;

//	int m_waitFrame;

	// 位置情報
	Vec2 m_pos;
	Vec2 m_lastPos;
	// 移動情報
	Vec2 m_vec;

	int m_hp;
	int m_Attack;

	//アニメーション管理用変数
	int m_animFrame;
	//左右反転フラグ
	bool m_isFlip;
	// 移動中かどうか
	bool m_isMoving;

	// 死亡フラグ
	bool m_isDead;

	// 登場回数
//	int m_apperCount;

//	bool m_isHit;

};
