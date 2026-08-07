#pragma once

class Scenebattle
{
public:
	Scenebattle();
	~Scenebattle();

	void Init();
	void End();
	void Update();
	void Draw();

	bool isFinished = false;

private:

	// バトルの状態
	enum class BattleState
	{
		Start,       // 戦闘開始
		PlayerAttack,// プレイヤーの攻撃
		EnemyAttack, // 敵の反撃
		Win,         // 勝利
		Lose         // 敗北
	};

	BattleState m_state;
	
	// パラメータ（仮）
	int m_playerHp;
	int m_playerMaxHp;
	int m_enemyHp;
	int m_enemyMaxHp;

	// ボタンのトリガー（押した瞬間だけ検知）用
	int m_oldPadInput;
};
