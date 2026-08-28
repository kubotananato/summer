#pragma once
#include "Player.h"

class Player;

class Scenebattle
{
public:
	Scenebattle();
	~Scenebattle();

	void Init(Player* pPlayer, bool isBoss = false);
	void End();
	void Update();
	void Draw();

	bool IsFinished() const { return isFinished; }
	bool isFinished;

	bool IsWin() const { return m_state == BattleState::Win; }
	bool IsLose() const { return m_state == BattleState::Lose; }

private:
	enum class BattleState
	{
		Start,
		SelectCommand,
		PlayerAttack,
		PlayerMagic,
		NoMp,
		EnemyAttack,
		Win,
		Lose
	};

	BattleState m_state;

	int m_gainedMaxHp = 0;
	int m_gainedMaxMp = 0;
	int m_gainedAtk = 0;
	bool m_isBoss = false;

	// 背景関連
	int m_bgImgHandle;

	// 敵関連
	static constexpr int kEnemyAnimFrames = 6;
	int m_enemyImgHandle[kEnemyAnimFrames];
	int m_animTimer;
	int m_enemyHp;
	int m_enemyMaxHp;
	int m_enemyAttack;
	static constexpr int kBossAnimFrames = 4;
	int m_bossImgHandle[kBossAnimFrames];

	// プレイヤー本体へのポインタ
	Player* m_pPlayer;

	// リザルト用
	int m_gainedExp;
	bool m_isLevelUp;

	// 入力保持用
	int m_oldPadInput;
	bool m_oldSpaceInput;
	int m_cursorIndex;

	// ダメージ値の保持用
	int m_lastDamageToEnemy;
	int m_lastDamageToPlayer;

private:
	struct DamagePopup
	{
		int damage = 0;
		float x = 0.0f;
		float y = 0.0f;
		int timer = 0;
		bool active = false;
	};
	DamagePopup m_enemyPopup;
	DamagePopup m_playerPopup;

	int CalculateDamage(int baseDamage, float variance = 0.2f);
	void SpawnDamagePopup(DamagePopup& popup, int damage, float x, float y);
};