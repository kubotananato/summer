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

	// 敵関連
	static constexpr int kEnemyAnimFrames = 6;
	int m_enemyImgHandle[kEnemyAnimFrames];
	int m_animTimer;
	int m_enemyHp;
	int m_enemyMaxHp;
	int m_enemyAttack;

	// プレイヤー本体へのポインタ
	Player* m_pPlayer;

	// リザルト用
	int m_gainedExp;
	bool m_isLevelUp;

	// 入力保持用
	int m_oldPadInput;
	bool m_oldSpaceInput;
	int m_cursorIndex;
};