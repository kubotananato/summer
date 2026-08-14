#include "Scenebattle.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"

namespace
{
	// UI描画用の色定数
	const unsigned int kColorWhite = GetColor(255, 255, 255);
	const unsigned int kColorBlack = GetColor(0, 0, 0);
	const unsigned int kColorRed = GetColor(220, 60, 60);
	const unsigned int kColorGreen = GetColor(60, 220, 60);
	const unsigned int kColorBlue = GetColor(60, 140, 255);
	const unsigned int kColorGray = GetColor(40, 40, 50);
	const unsigned int kColorYellow = GetColor(255, 255, 0);

	constexpr int kCommandCount = 3;
}

Scenebattle::Scenebattle() :
	isFinished(false),
	m_state(BattleState::Start),
	m_enemyHp(50),
	m_enemyMaxHp(50),
	m_enemyAttack(10),
	m_oldPadInput(0),
	m_oldSpaceInput(false),
	m_cursorIndex(0),
	m_animTimer(0),
	m_pPlayer(nullptr),
	m_gainedExp(0),
	m_isLevelUp(false)
{
	for (int i = 0; i < kEnemyAnimFrames; i++)
	{
		m_enemyImgHandle[i] = -1;
	}
}

Scenebattle::~Scenebattle()
{
}

void Scenebattle::Init(Player* pPlayer, bool isBoss)
{
	isFinished = false;
	m_state = BattleState::Start;

	m_pPlayer = pPlayer;
	m_isBoss = isBoss;

	if (m_isBoss)
	{
		m_enemyMaxHp = 250;     // ボスの最大HP
		m_enemyAttack = 25;     // ボスの攻撃力
		m_gainedExp = 200;      // ボス撃破時の経験値
	}
	else
	{
		m_enemyMaxHp = 50;      // 通常モンスターの最大HP
		m_enemyAttack = 10;     // 通常モンスターの攻撃力
		m_gainedExp = 45;       // 通常モンスターの経験値
	}

	// ★ 修正：現在のHPを最大HPでリセット
	m_enemyHp = m_enemyMaxHp;

	m_cursorIndex = 0;
	m_animTimer = 0;
	m_isLevelUp = false;

	m_gainedMaxHp = 0;
	m_gainedMaxMp = 0;
	m_gainedAtk = 0;

	m_oldPadInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	m_oldSpaceInput = (CheckHitKey(KEY_INPUT_SPACE) == 1);

	if (m_enemyImgHandle[0] == -1)
	{
		LoadDivGraph("data/Enemy/EIdle.png", 6, 6, 1, 32, 32, m_enemyImgHandle);
	}
}

void Scenebattle::End()
{
	for (int i = 0; i < kEnemyAnimFrames; i++)
	{
		if (m_enemyImgHandle[i] != -1)
		{
			DeleteGraph(m_enemyImgHandle[i]);
			m_enemyImgHandle[i] = -1;
		}
	}
}

void Scenebattle::Update()
{
	if (m_pPlayer == nullptr) return;

	m_animTimer++;

	// 入力判定（押しっぱなし防止のトリガー処理）
	int currentPadInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	int pushPad = currentPadInput & ~m_oldPadInput;
	m_oldPadInput = currentPadInput;

	bool currentSpaceInput = (CheckHitKey(KEY_INPUT_SPACE) == 1);
	bool pushSpace = currentSpaceInput && !m_oldSpaceInput;
	m_oldSpaceInput = currentSpaceInput;

	bool isPressed = (pushPad & PAD_INPUT_1) || (pushPad & PAD_INPUT_2) || (pushPad & PAD_INPUT_10) || pushSpace;
	bool isUp = (pushPad & PAD_INPUT_UP) != 0;
	bool isDown = (pushPad & PAD_INPUT_DOWN) != 0;

	// コマンド選択中のカーソル移動
	if (m_state == BattleState::SelectCommand)
	{
		if (isUp)
		{
			m_cursorIndex--;
			if (m_cursorIndex < 0) m_cursorIndex = kCommandCount - 1;
		}
		if (isDown)
		{
			m_cursorIndex++;
			if (m_cursorIndex >= kCommandCount) m_cursorIndex = 0;
		}
	}

	if (isPressed)
	{
		switch (m_state)
		{
		case BattleState::Start:
			m_state = BattleState::SelectCommand;
			break;

		case BattleState::SelectCommand:
			if (m_cursorIndex == 0) // こうげき
			{
				m_state = BattleState::PlayerAttack;
				m_enemyHp -= (m_pPlayer->GetAttack() + 5);
			}
			else if (m_cursorIndex == 1) // じゅもん
			{
				if (m_pPlayer->ConsumeMp(10))
				{
					m_state = BattleState::PlayerMagic;
					m_enemyHp -= (m_pPlayer->GetAttack() * 2);
				}
				else
				{
					m_state = BattleState::NoMp;
				}
			}
			else if (m_cursorIndex == 2) // にげる
			{
				isFinished = true;
			}

			// 敵撃破判定
			if (m_enemyHp <= 0 && m_state != BattleState::NoMp)
			{
				m_enemyHp = 0;
				m_state = BattleState::Win;

				int oldMaxHp = m_pPlayer->GetMaxHp();
				int oldMaxMp = m_pPlayer->GetMaxMp();
				int oldAtk = m_pPlayer->GetAttack();

				// Initで設定された m_gainedExp をそのまま使用
				m_isLevelUp = m_pPlayer->GainExp(m_gainedExp);

				if (m_isLevelUp)
				{
					m_gainedMaxHp = m_pPlayer->GetMaxHp() - oldMaxHp;
					m_gainedMaxMp = m_pPlayer->GetMaxMp() - oldMaxMp;
					m_gainedAtk = m_pPlayer->GetAttack() - oldAtk;
				}
			}
			break;

		case BattleState::NoMp:
			m_state = BattleState::SelectCommand;
			break;

		case BattleState::PlayerAttack:
		case BattleState::PlayerMagic:
			m_state = BattleState::EnemyAttack;

			m_pPlayer->TakeDamage(m_enemyAttack);
			if (m_pPlayer->IsDead())
			{
				m_state = BattleState::Lose;
			}
			break;

		case BattleState::EnemyAttack:
			m_state = BattleState::SelectCommand;
			break;

		case BattleState::Win:
		case BattleState::Lose:
			isFinished = true;
			break;
		}
	}
}

void Scenebattle::Draw()
{
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(15, 15, 25), TRUE);

	// 敵表示位置
	int enemyCenterX = Game::kScreenWidth / 2;
	int enemyCenterY = 220;

	if (m_enemyHp > 0)
	{
		if (m_isBoss)
		{
			// ボス：大きめの赤丸を描画（半径90）
			DrawCircle(enemyCenterX, enemyCenterY, 90, kColorRed, TRUE);
		}
		else
		{
			// 通常モンスター：画像があれば表示、なければ赤丸（半径50）
			if (m_enemyImgHandle[0] != -1)
			{
				int currentFrame = (m_animTimer / 10) % kEnemyAnimFrames;
				DrawRotaGraph(enemyCenterX, enemyCenterY, 3.0, 0.0, m_enemyImgHandle[currentFrame], TRUE);
			}
			else
			{
				DrawCircle(enemyCenterX, enemyCenterY, 50, kColorRed, TRUE);
			}
		}
	}

	// 敵HPバー
	int barWidth = 200;
	int barHeight = 15;
	int barX = enemyCenterX - (barWidth / 2);
	int barY = enemyCenterY + 110;
	DrawBox(barX, barY, barX + barWidth, barY + barHeight, kColorGray, TRUE);
	int currentEnemyBarWidth = (barWidth * m_enemyHp) / m_enemyMaxHp;
	DrawBox(barX, barY, barX + currentEnemyBarWidth, barY + barHeight, kColorRed, TRUE);
	DrawFormatString(barX + 25, barY - 20, kColorWhite, "敵 HP: %d / %d", m_enemyHp, m_enemyMaxHp);

	// プレイヤーUI
	int pBarX = 80;
	int pBarY = 430;

	if (m_pPlayer != nullptr)
	{
		DrawFormatString(pBarX, pBarY - 25, kColorYellow, "Lv.%d  (攻撃力:%d)",
			m_pPlayer->GetLevel(), m_pPlayer->GetAttack());

		DrawFormatString(pBarX, pBarY, kColorWhite, "HP: %d / %d", m_pPlayer->GetHp(), m_pPlayer->GetMaxHp());
		DrawBox(pBarX, pBarY + 20, pBarX + 200, pBarY + 30, kColorGray, TRUE);
		int pHpWidth = (200 * m_pPlayer->GetHp()) / m_pPlayer->GetMaxHp();
		DrawBox(pBarX, pBarY + 20, pBarX + pHpWidth, pBarY + 30, kColorGreen, TRUE);

		DrawFormatString(pBarX, pBarY + 40, kColorWhite, "MP: %d / %d", m_pPlayer->GetMp(), m_pPlayer->GetMaxMp());
		DrawBox(pBarX, pBarY + 60, pBarX + 200, pBarY + 70, kColorGray, TRUE);
		int pMpWidth = (m_pPlayer->GetMaxMp() > 0) ? (200 * m_pPlayer->GetMp()) / m_pPlayer->GetMaxMp() : 0;
		DrawBox(pBarX, pBarY + 60, pBarX + pMpWidth, pBarY + 70, kColorBlue, TRUE);
	}

	// メッセージウィンドウ & コマンドウィンドウ
	int winX1 = 50;
	int winY1 = 530;
	int winX2 = Game::kScreenWidth - 50;
	int winY2 = Game::kScreenHeight - 30;

	DrawBox(winX1, winY1, winX2, winY2, kColorBlack, TRUE);
	DrawBox(winX1, winY1, winX2, winY2, kColorWhite, FALSE);

	int textX = winX1 + 30;
	int textY = winY1 + 25;

	if (m_state == BattleState::SelectCommand)
	{
		DrawString(textX, textY, "どうする？", kColorWhite);

		int cmdX1 = Game::kScreenWidth - 250;
		int cmdY1 = winY1;
		int cmdX2 = winX2;
		int cmdY2 = winY2;

		DrawBox(cmdX1, cmdY1, cmdX2, cmdY2, kColorBlack, TRUE);
		DrawBox(cmdX1, cmdY1, cmdX2, cmdY2, kColorWhite, FALSE);

		const char* commands[] = { "こうげき", "じゅもん(MP10)", "にげる" };
		for (int i = 0; i < kCommandCount; i++)
		{
			int itemY = cmdY1 + 15 + (i * 28);

			if (i == m_cursorIndex)
			{
				DrawString(cmdX1 + 15, itemY, "＞", kColorYellow);
				DrawString(cmdX1 + 35, itemY, commands[i], kColorYellow);
			}
			else
			{
				DrawString(cmdX1 + 35, itemY, commands[i], kColorWhite);
			}
		}
	}
	else
	{
		switch (m_state)
		{
		case BattleState::Start:
			if (m_isBoss)
				DrawString(textX, textY, "ボスが現れた！", kColorWhite);
			else
				DrawString(textX, textY, "野生のモンスターが現れた！", kColorWhite);
			break;

		case BattleState::PlayerAttack:
			if (m_pPlayer != nullptr)
			{
				DrawFormatString(textX, textY, kColorWhite, "プレイヤーのこうげき！ モンスターに %d のダメージ！", m_pPlayer->GetAttack() + 5);
			}
			break;

		case BattleState::PlayerMagic:
			if (m_pPlayer != nullptr)
			{
				DrawFormatString(textX, textY, kColorWhite, "プレイヤーは呪文を唱えた！ モンスターに %d のダメージ！", m_pPlayer->GetAttack() * 2);
			}
			break;

		case BattleState::NoMp:
			DrawString(textX, textY, "MPが 足りない！", GetColor(255, 100, 100));
			break;

		case BattleState::EnemyAttack:
			DrawFormatString(textX, textY, kColorWhite, "モンスターの反撃！ %d のダメージを受けた！", m_enemyAttack);
			break;

		case BattleState::Win:
			DrawFormatString(textX, textY, GetColor(100, 255, 100), "モンスターを倒した！ %d の経験値を獲得！", m_gainedExp);

			if (m_isLevelUp && m_pPlayer != nullptr)
			{
				DrawFormatString(textX, textY + 28, kColorYellow,
					"★ レベルがあがった！ (Lv.%d)", m_pPlayer->GetLevel());

				DrawFormatString(textX, textY + 56, kColorYellow,
					"   最大HP+%d  最大MP+%d  攻撃力+%d", m_gainedMaxHp, m_gainedMaxMp, m_gainedAtk);
			}
			break;

		case BattleState::Lose:
			DrawString(textX, textY, "プレイヤーは倒れてしまった…", kColorRed);
			break;
		}

		DrawString(textX, winY2 - 30, "[ ボタン / SPACE ] つぎへ", GetColor(200, 200, 200));
	}
}