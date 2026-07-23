#pragma once
#include "Vec2.h"

class Player
{
public:
	// 方向をあらわす列挙型
	enum class Direction
	{
		Down,
		Left,
		Right,
		Up
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


	// 位置取得
	Vec2 GetPos() const { return m_pos; }

	// 当たり判定用
	Vec2 GetColCenter() const;
	float GetColRadius() const;

	// HPの取得
	int GetHp() const { return m_hp; }

	// 敵の攻撃を食らったときの処理
//	void OnHit();

	bool IsDead() const;

private:
	Direction m_dir = Direction::Right; // 初期向き

	// グラフィックハンドル
	int m_idleHandle;
	//アニメーション管理用変数
	int m_animFrame;
	//左右反転フラグ
	bool m_isFlip;

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
	// 最大MP
	int m_maxMp;
	// 移動速度
	int m_Speed;
	// 死亡フラグ
	bool m_isDead;
};

