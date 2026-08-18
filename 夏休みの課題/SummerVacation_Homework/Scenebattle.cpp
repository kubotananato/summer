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

	// ドラクエ風ウィンドウを描画する共通関数
	void DrawDQWindow(int x1, int y1, int x2, int y2)
	{
		// 背景の黒塗り
		DrawBox(x1, y1, x2, y2, kColorBlack, TRUE);

		// 外枠・内枠
		DrawBox(x1, y1, x2, y2, kColorWhite, FALSE);
		DrawBox(x1 + 3, y1 + 3, x2 - 3, y2 - 3, kColorWhite, FALSE);

		DrawPixel(x1, y1, kColorBlack);
		DrawPixel(x2, y1, kColorBlack);
		DrawPixel(x1, y2, kColorBlack);
		DrawPixel(x2, y2, kColorBlack);

		DrawPixel(x1 + 3, y1 + 3, kColorBlack);
		DrawPixel(x2 + 3, y1 + 3, kColorBlack);
		DrawPixel(x1 + 3, y2 - 3, kColorBlack);
		DrawPixel(x2 + 3, y2 - 3, kColorBlack);
	}
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
	m_isLevelUp(false),
	m_lastDamageToEnemy(0),
	m_lastDamageToPlayer(0),
	m_enemyPopup(),
	m_playerPopup()
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

	// ダメージポップアップの更新処理
	if (m_enemyPopup.active)
	{
		m_enemyPopup.timer--;
		m_enemyPopup.y -= 0.8f;
		if (m_enemyPopup.timer <= 0) m_enemyPopup.active = false;
	}

	if (m_playerPopup.active)
	{
		m_playerPopup.timer--;
		m_playerPopup.y -= 0.8f;
		if (m_playerPopup.timer <= 0) m_playerPopup.active = false;
	}

	// 入力判定（トリガー処理）
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
				m_lastDamageToEnemy = CalculateDamage(m_pPlayer->GetAttack() + 5, 0.2f);
				m_enemyHp -= m_lastDamageToEnemy;

				int enemyCenterX = (150 + Game::kScreenWidth - 30) / 2;
				SpawnDamagePopup(m_enemyPopup, m_lastDamageToEnemy, static_cast<float>(enemyCenterX - 10), 100.0f);
			}
			else if (m_cursorIndex == 1) // じゅもん
			{
				if (m_pPlayer->ConsumeMp(10))
				{
					m_state = BattleState::PlayerMagic;
					m_lastDamageToEnemy = CalculateDamage(m_pPlayer->GetAttack() * 2, 0.15f);
					m_enemyHp -= m_lastDamageToEnemy;

					int enemyCenterX = (150 + Game::kScreenWidth - 30) / 2;
					SpawnDamagePopup(m_enemyPopup, m_lastDamageToEnemy, static_cast<float>(enemyCenterX - 10), 100);
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

			// ランダムダメージの計算
			m_lastDamageToPlayer = CalculateDamage(m_enemyAttack, 0.2f);
			m_pPlayer->TakeDamage(m_lastDamageToPlayer);

			SpawnDamagePopup(m_playerPopup, m_lastDamageToPlayer, 80, 50);

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
	// 背景の暗転表示
	DrawBox(0, 0, Game::kScreenWidth, Game::kScreenHeight, GetColor(15, 15, 25), TRUE);

	// ステータスウィンドウ
	int statusX1 = 30;
	int statusY1 = 20;
	int statusX2 = 140;
	int statusY2 = 135;

	DrawDQWindow(statusX1, statusY1, statusX2, statusY2);

	if (m_pPlayer != nullptr)
	{
		DrawString(statusX1 + 15, statusY1 + 12, "ゆうしゃ", kColorWhite);
		DrawFormatString(statusX1 + 15, statusY1 + 36, kColorWhite, "H %3d", m_pPlayer->GetHp());
		DrawFormatString(statusX1 + 15, statusY1 + 58, kColorWhite, "M %3d", m_pPlayer->GetMp());
		DrawFormatString(statusX1 + 15, statusY1 + 80, kColorWhite, "Lv %2d", m_pPlayer->GetLevel());
	}

	// 戦闘画面枠
	int viewX1 = 150;
	int viewY1 = 20;
	int viewX2 = Game::kScreenWidth - 30;
	int viewY2 = 320;

	// 外側の白い太枠 背景塗り
	DrawBox(viewX1, viewY1, viewX2, viewY2, kColorWhite, TRUE);
	DrawBox(viewX1 + 3, viewY1 + 3, viewX2 - 3, viewY2 - 3, GetColor(10, 15, 30), TRUE);

	// 敵表示
	int enemyCenterX = (viewX1 + viewX2) / 2;
	int enemyCenterY = (viewY1 + viewY2) / 2 - 15;

	if (m_enemyHp > 0)
	{
		if (m_isBoss)
		{
			DrawCircle(enemyCenterX, enemyCenterY, 80, kColorRed, TRUE);
		}
		else
		{
			if (m_enemyImgHandle[0] != -1)
			{
				int currentFrame = (m_animTimer / 10) % kEnemyAnimFrames;
				DrawRotaGraph(enemyCenterX, enemyCenterY, 3.5, 0.0, m_enemyImgHandle[currentFrame], TRUE);
			}
			else
			{
				DrawCircle(enemyCenterX, enemyCenterY, 45, kColorRed, TRUE);
			}
		}

		// 敵HPバー
		int barWidth = 160;
		int barHeight = 10;
		int barX = enemyCenterX - (barWidth / 2);
		int barY = viewY2 - 25;

		DrawBox(barX, barY, barX + barWidth, barY + barHeight, kColorGray, TRUE);
		int currentEnemyBarWidth = (barWidth * m_enemyHp) / m_enemyMaxHp;
		DrawBox(barX, barY, barX + currentEnemyBarWidth, barY + barHeight, kColorRed, TRUE);
	}

	// コマンドウィンドウ
	int cmdX1 = 30;
	int cmdY1 = 335;
	int cmdX2 = 180;
	int cmdY2 = Game::kScreenHeight - 20;

	DrawDQWindow(cmdX1, cmdY1, cmdX2, cmdY2);

	DrawString(cmdX1 + 15, cmdY1 + 12, "ゆうしゃ", kColorWhite);
	DrawLine(cmdX1 + 8, cmdY1 + 32, cmdX2 - 8, cmdY1 + 32, kColorWhite);

	const char* commands[] = { "こうげき", "じゅもん", "にげる" };
	for (int i = 0; i < kCommandCount; i++)
	{
		int itemY = cmdY1 + 42 + (i * 26);

		if (m_state == BattleState::SelectCommand && i == m_cursorIndex)
		{
			DrawString(cmdX1 + 12, itemY, ">", kColorYellow);
			DrawString(cmdX1 + 30, itemY, commands[i], kColorYellow);
		}
		else
		{
			DrawString(cmdX1 + 30, itemY, commands[i], kColorWhite);
		}
	}

	// ダメージポップアップの描画
	if (m_enemyPopup.active)
	{
		DrawFormatString(static_cast<int>(m_enemyPopup.x), static_cast<int>(m_enemyPopup.y), kColorYellow, "%d", m_enemyPopup.damage);
	}

	if (m_playerPopup.active)
	{
		DrawFormatString(static_cast<int>(m_playerPopup.x), static_cast<int>(m_playerPopup.y), kColorRed, "%d", m_playerPopup.damage);
	}

	// メッセージ / 敵情報ウィンドウ
	int msgX1 = 190;
	int msgY1 = 335;
	int msgX2 = Game::kScreenWidth - 30;
	int msgY2 = Game::kScreenHeight - 20;

	DrawDQWindow(msgX1, msgY1, msgX2, msgY2);

	int textX = msgX1 + 20;
	int textY = msgY1 + 18;

	if (m_state == BattleState::SelectCommand)
	{
		// コマンド選択時：出現中の敵の名前と匹数を表示
		const char* enemyName = m_isBoss ? "ボスモンスター" : "モンスター";
		DrawString(textX, textY, enemyName, kColorWhite);
		DrawString(textX + 200, textY, "1匹", kColorWhite);
	}
	else
	{
		// 戦闘中のテキストメッセージ表示
		switch (m_state)
		{
		case BattleState::Start:
			if (m_isBoss)
				DrawString(textX, textY, "ボスが現れた！", kColorWhite);
			else
				DrawString(textX, textY, "モンスターが現れた！", kColorWhite);
			break;

		case BattleState::PlayerAttack:
			if (m_pPlayer != nullptr)
			{
				DrawFormatString(textX, textY, kColorWhite, "プレイヤーのこうげき！\n\nモンスターに %d のダメージ！", m_lastDamageToEnemy);
			}
			break;

		case BattleState::PlayerMagic:
			if (m_pPlayer != nullptr)
			{
				DrawFormatString(textX, textY, kColorWhite, "プレイヤーは じゅもんを唱えた！\n\nモンスターに %d のダメージ！", m_lastDamageToEnemy);
			}
			break;

		case BattleState::NoMp:
			DrawString(textX, textY, "MPが 足りない！", GetColor(255, 100, 100));
			break;

		case BattleState::EnemyAttack:
			DrawFormatString(textX, textY, kColorWhite, "モンスターの反撃！\n\n%d のダメージを受けた！", m_lastDamageToPlayer);
			break;

		case BattleState::Win:
			DrawFormatString(textX, textY, GetColor(100, 255, 100), "モンスターを倒した！\n%d の経験値を獲得！", m_gainedExp);

			if (m_isLevelUp && m_pPlayer != nullptr)
			{
				DrawFormatString(textX, textY + 42, kColorYellow, "★ レベルがあがった！ (Lv.%d)", m_pPlayer->GetLevel());
				DrawFormatString(textX, textY + 64, kColorYellow, "   HP+%d  MP+%d  攻撃力+%d", m_gainedMaxHp, m_gainedMaxMp, m_gainedAtk);
			}
			break;

		case BattleState::Lose:
			DrawString(textX, textY, "プレイヤーは倒れてしまった…", kColorRed);
			break;
		}

		// 進行案内テキスト
		DrawString(textX, msgY2 - 25, "[ SPACE / BUTTON ] つぎへ", GetColor(180, 180, 180));
	}
}

// ダメージのランダム化
int Scenebattle::CalculateDamage(int baseDamage, float variance)
{
	int minDmg = static_cast<int>(baseDamage * (1.0f - variance));
	int maxDmg = static_cast<int>(baseDamage * (1.0f + variance));

	if (minDmg < 1) minDmg = 1;
	if (maxDmg < minDmg) maxDmg = minDmg;

	return minDmg + GetRand(maxDmg - minDmg);
}

// ダメージテキストの生成
void Scenebattle::SpawnDamagePopup(DamagePopup& popup, int damage, float x, float y)
{
	popup.damage = damage;
	popup.x = x;
	popup.y = y;
	popup.timer = 40; // 40フレーム（約0.6秒）表示
	popup.active = true;
}