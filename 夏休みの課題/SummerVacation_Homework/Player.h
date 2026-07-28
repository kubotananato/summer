#pragma once
#include "Vec2.h"

class Player
{
public:
	// 方向をあらわす列挙型
	enum class Direction
	{
		Down,  // 下向き
		Left,  // 左向き
		Right, // 右向き
		Up     // 上向き
	};

public:
	Player();
	~Player();

	void Init();
	void End();
	void Update();
	void Draw();

	// グラフィックの設定
	void SetIdleHandle(int handle) { m_idleHandle = handle; }
	void SetRunHandle(int handle) { m_runHandle = handle; }

	// 位置取得
	Vec2 GetPos() const { return m_pos; }

	// 当たり判定用
	Vec2 GetColCenter() const;
	float GetColRadius() const;

	// HPの取得
	int GetHp() const { return m_hp; }
	int GetMp() const { return m_mp; }

	// 向きの取得
	Direction GetDir() const { return m_dir; }

	bool IsDead() const;

	void FullHeal();

private:
	// 向き管理
	Direction m_dir;

	// グラフィックハンドル
	int m_idleHandle;
	int m_runHandle;

	// アニメーション管理用変数
	int m_animFrame;

	// 移動中かどうか
	bool m_isMoving;

	// 現在位置
	Vec2 m_pos;
	// 移動情報
	Vec2 m_vec;

	float m_angle;

	int m_attack;

	// 地面についているかどうか
	bool m_isLanding;

	// 残りHP
	int m_hp;
	// 残りMP
	int m_mp;
	// 最大HP
	int m_maxHp;
	// 最大MP
	int m_maxMp;
	// 移動速度
	int m_Speed;
	// 死亡フラグ
	bool m_isDead;
};
