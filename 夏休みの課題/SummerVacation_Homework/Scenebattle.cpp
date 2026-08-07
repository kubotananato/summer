#include "Scenebattle.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	// UI描画用の色定数
	const unsigned int kColorWhite = GetColor(255, 255, 255);
	const unsigned int kColorBlack = GetColor(0, 0, 0);
	const unsigned int kColorRed = GetColor(220, 60, 60);
	const unsigned int kColorGreen = GetColor(60, 220, 60);
	const unsigned int kColorGray = GetColor(40, 40, 50);
}

Scenebattle::Scenebattle() :
	isFinished(false),
	m_state(BattleState::Start),
	m_playerHp(100),
	m_playerMaxHp(100),
	m_enemyHp(50),
	m_enemyMaxHp(50),
	m_oldPadInput(0)
{
}

Scenebattle::~Scenebattle()
{
}

void Scenebattle::Init()
{
	isFinished = false;
	m_state = BattleState::Start;

	// バトル開始時のステータス初期化
	m_playerMaxHp = 100;
	m_playerHp = 100;
	m_enemyMaxHp = 50;
	m_enemyHp = m_enemyMaxHp;

	m_oldPadInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}

void Scenebattle::End()
{
}

void Scenebattle::Update()
{
	int currentPadInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);

	// ボタンが「押された瞬間」だけを判定（連打防止）
	bool isPressed = false;
	if ((currentPadInput & PAD_INPUT_1) && !(m_oldPadInput & PAD_INPUT_1)) isPressed = true;
	if ((currentPadInput & PAD_INPUT_10) && !(m_oldPadInput & PAD_INPUT_10)) isPressed = true;

	// キーボード（SPACEキー）のトリガー判定
	static bool oldSpace = false;
	bool currentSpace = (CheckHitKey(KEY_INPUT_SPACE) != 0);
	if (currentSpace && !oldSpace)
	{
		isPressed = true;
	}
	oldSpace = currentSpace;

	m_oldPadInput = currentPadInput;

	// ボタンが押されたら状態を次に進める
	if (isPressed)
	{
		switch (m_state)
		{
		case BattleState::Start:
			// 攻撃へ移行
			m_state = BattleState::PlayerAttack;
			m_enemyHp -= 20; // プレイヤーの攻撃（20ダメージ）
			if (m_enemyHp <= 0)
			{
				m_enemyHp = 0;
				m_state = BattleState::Win;
			}
			break;

		case BattleState::PlayerAttack:
			// 敵の反撃へ移行
			m_state = BattleState::EnemyAttack;
			m_playerHp -= 10; // 敵の攻撃（10ダメージ）
			if (m_playerHp <= 0)
			{
				m_playerHp = 0;
				m_state = BattleState::Lose;
			}
			break;

		case BattleState::EnemyAttack:
			// 再びプレイヤーの攻撃へ
			m_state = BattleState::PlayerAttack;
			m_enemyHp -= 20;
			if (m_enemyHp <= 0)
			{
				m_enemyHp = 0;
				m_state = BattleState::Win;
			}
			break;

		case BattleState::Win:
		case BattleState::Lose:
			// 勝敗が決まった後にボタンを押したらフィールドに戻る
			isFinished = true;
			break;
		}
	}
}

void Scenebattle::Draw()
{
	//  背景の暗転表示
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(15, 15, 25), TRUE);

	//  敵の仮表示（円または四角形）
	int enemyCenterX = Game::kScreenWidth / 2;
	int enemyCenterY = 220;
	if (m_enemyHp > 0)
	{
		DrawCircle(enemyCenterX, enemyCenterY, 60, kColorRed, TRUE);
		DrawString(enemyCenterX - 30, enemyCenterY - 10, "モンスター", kColorWhite);
	}

	// 敵のHPバー表示
	int barWidth = 200;
	int barHeight = 15;
	int barX = enemyCenterX - (barWidth / 2);
	int barY = enemyCenterY + 80;

	DrawBox(barX, barY, barX + barWidth, barY + barHeight, kColorGray, TRUE);
	int currentEnemyBarWidth = (barWidth * m_enemyHp) / m_enemyMaxHp;
	DrawBox(barX, barY, barX + currentEnemyBarWidth, barY + barHeight, kColorRed, TRUE);
	DrawFormatString(barX, barY - 20, kColorWhite, "敵 HP: %d / %d", m_enemyHp, m_enemyMaxHp);

	// プレイヤーのHPバー表示（左下）
	int pBarX = 80;
	int pBarY = 480;
	DrawFormatString(pBarX, pBarY - 25, kColorWhite, "プレイヤー HP: %d / %d", m_playerHp, m_playerMaxHp);
	DrawBox(pBarX, pBarY, pBarX + 250, pBarY + 15, kColorGray, TRUE);
	int currentPlayerBarWidth = (250 * m_playerHp) / m_playerMaxHp;
	DrawBox(pBarX, pBarY, pBarX + currentPlayerBarWidth, pBarY + 15, kColorGreen, TRUE);

	// メッセージウィンドウ（画面下部）
	int winX1 = 50;
	int winY1 = 520;
	int winX2 = Game::kScreenWidth - 50;
	int winY2 = Game::kScreenHeight - 40;

	// 枠と背景
	DrawBox(winX1, winY1, winX2, winY2, kColorBlack, TRUE);
	DrawBox(winX1, winY1, winX2, winY2, kColorWhite, FALSE);

	// 状態に応じたテキスト描画
	int textX = winX1 + 30;
	int textY = winY1 + 30;

	switch (m_state)
	{
	case BattleState::Start:
		DrawString(textX, textY, "野生のモンスターが現れた！", kColorWhite);
		DrawString(textX, textY + 40, "[ SPACE / Aボタン ] で攻撃！", GetColor(200, 200, 100));
		break;

	case BattleState::PlayerAttack:
		DrawString(textX, textY, "プレイヤーの攻撃！ モンスターに 20 のダメージ！", kColorWhite);
		DrawString(textX, textY + 40, "[ SPACE / Aボタン ] 次へ", GetColor(200, 200, 100));
		break;

	case BattleState::EnemyAttack:
		DrawString(textX, textY, "モンスターの反撃！ プレイヤーは 10 のダメージを受けた！", kColorWhite);
		DrawString(textX, textY + 40, "[ SPACE / Aボタン ] 次へ", GetColor(200, 200, 100));
		break;

	case BattleState::Win:
		DrawString(textX, textY, "モンスターを倒した！ 勝利！", GetColor(100, 255, 100));
		DrawString(textX, textY + 40, "[ SPACE / Aボタン ] フィールドに戻る", kColorWhite);
		break;

	case BattleState::Lose:
		DrawString(textX, textY, "プレイヤーは倒れてしまった…", kColorRed);
		DrawString(textX, textY + 40, "[ SPACE / Aボタン ] フィールドに戻る", kColorWhite);
		break;
	}
}