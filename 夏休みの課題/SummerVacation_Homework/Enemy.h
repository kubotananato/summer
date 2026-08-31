#pragma once
#include "Vec2.h"
#include "Map.h"

class Enemy
{
public:
	Enemy();
	~Enemy();

	void Init();
	void End();
	void Update(const Vec2& playerPos, const Map& map);
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

	void SetPos(const Vec2& pos) { m_pos = pos; }

private:
	// AIの状態定義
	enum class State
	{
		Wander, // 徘徊
		Chase   // 追尾
	};

	// AI用ヘルパー関数
	bool CanSeePlayer(const Vec2& toPlayer, float dist) const;
	void UpdateWander();
	void UpdateChase(const Vec2& toPlayer, float dist);

private:
	// グラフィックハンドル
	int m_IdleHandle;
	int m_RunHandle;

	// 位置・移動情報
	Vec2 m_pos;
	Vec2 m_lastPos;
	Vec2 m_vec;
	Vec2 m_dir; // 向いている方向（単位ベクトル）

	// AI関連
	State m_state;
	int m_wanderTimer;

	int m_hp;
	int m_Attack;

	// アニメーション管理用変数
	int m_animFrame;
	// 左右反転フラグ
	bool m_isFlip;
	// 移動中かどうか
	bool m_isMoving;

	// 死亡フラグ
	bool m_isDead;
};