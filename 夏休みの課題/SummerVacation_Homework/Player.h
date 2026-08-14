#pragma once

#include "Vec2.h"

class Map;

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
	void Update(const Map& map);
	void Draw();

	// グラフィックの設定
	void SetIdleHandle(int handle) { m_idleHandle = handle; }
	void SetRunHandle(int handle) { m_runHandle = handle; }

	// 位置取得
	Vec2 GetPos() const { return m_pos; }

	// 当たり判定用
	Vec2 GetColCenter() const;
	float GetColRadius() const;

	// ステータス取得
	int GetLevel() const { return m_level; }
	int GetHp() const { return m_hp; }
	int GetMaxHp() const { return m_maxHp; }
	int GetMp() const { return m_mp; }
	int GetMaxMp() const { return m_maxMp; }
	int GetAttack() const { return m_attack; }

	// 向きの取得
	Direction GetDir() const { return m_dir; }

	// 生存フラグ
	bool IsDead() const;

	// 更新：戦闘・回復用関数 
	void TakeDamage(int damage);
	bool ConsumeMp(int cost);  // MP消費（足りていれば true）
	bool GainExp(int exp);     // 経験値獲得（レベルアップしたら true）
	void FullHeal();           // HP/MP全回復

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

	// 現在位置・移動情報
	Vec2 m_pos;
	Vec2 m_vec;

	float m_angle;

	// 地面についているかどうか
	bool m_isLanding;

	// ステータス変数
	int m_level;        // レベル
	int m_exp;          // 累積経験値
	int m_nextLevelExp; // 次のレベルに必要な経験値

	int m_hp;
	int m_maxHp;
	int m_mp;
	int m_maxMp;
	int m_attack;
	int m_Speed;

	// 死亡フラグ
	bool m_isDead;
};
