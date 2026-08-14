#include "Player.h"
#include "DxLib.h"
#include "Game.h"
#include "Map.h"

namespace
{
	// グラフィックの切り取り
	constexpr int kWidth = 32;
	constexpr int kHeight = 32;

	// 初期Status
	constexpr int kDefaultHP = 100;
	constexpr int kDefaultMP = 100;
	constexpr int kDefaultAttack = 15;
	constexpr int kSpeed = 4;

	constexpr int kWalkAnimNum = 3;
	constexpr int kSingleAnimFrame = 6; // 1コマにかけるフレーム数
	constexpr int kWalkAnimTotalFrame = kWalkAnimNum * kSingleAnimFrame;

	// 当たり判定の半径
	constexpr float kColRadius = 12.0f;
}

Player::Player() :
	m_idleHandle(-1),
	m_runHandle(-1),
	m_animFrame(0),
	m_isMoving(false),
	m_angle(0.0f),
	m_isLanding(false),
	m_level(1),
	m_exp(0),
	m_nextLevelExp(30),
	m_attack(0),
	m_hp(0),
	m_mp(0),
	m_maxHp(0),
	m_maxMp(0),
	m_Speed(0),
	m_isDead(false),
	m_dir(Direction::Down)
{
}

Player::~Player()
{
}

void Player::Init()
{
	m_pos.x = 100.f;
	m_pos.y = 100.f;

	m_vec.x = 0.0f;
	m_vec.y = 0.0f;

	m_animFrame = 0;
	m_isDead = false;
	m_isMoving = false;
	m_dir = Direction::Down; // 初期位置

	// --- ★ステータスの初期化 ---
	m_level = 1;
	m_exp = 0;
	m_nextLevelExp = 30; // 最初にレベル2に必要な経験値

	m_maxHp = kDefaultHP;
	m_hp = m_maxHp;

	m_maxMp = kDefaultMP;
	m_mp = m_maxMp;

	m_attack = kDefaultAttack;
	m_Speed = kSpeed;
}

void Player::End()
{
}

void Player::Update(const Map& map)
{
	// コントローラー・キーボード入力でキャラクターを移動させる
	if (!m_isDead)
	{
		int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		m_vec.x = 0.0f;
		m_vec.y = 0.0f;
		m_isMoving = false;

		if (pad & PAD_INPUT_UP)
		{
			m_vec.y = -1.0f;
			m_dir = Direction::Up;     // 上向き
			m_isMoving = true;
		}
		else if (pad & PAD_INPUT_DOWN)
		{
			m_vec.y = 1.0f;
			m_dir = Direction::Down;   // 下向き
			m_isMoving = true;
		}
		else if (pad & PAD_INPUT_LEFT)
		{
			m_vec.x = -1.0f;
			m_dir = Direction::Left;   // 左向き
			m_isMoving = true;
		}
		else if (pad & PAD_INPUT_RIGHT)
		{
			m_vec.x = 1.0f;
			m_dir = Direction::Right;  // 右向き
			m_isMoving = true;
		}

		// 移動中だけアニメーションのフレームを進める
		if (m_isMoving)
		{
			m_animFrame++;
		}
		else
		{
			m_animFrame = kSingleAnimFrame;
		}

		// --------------------------------------------------
		// 壁判定付きの移動処理
		// --------------------------------------------------

		// X軸方向の移動チェック
		if (m_vec.x != 0.0f)
		{
			float nextX = m_pos.x + m_vec.x * m_Speed;

			int leftTile = static_cast<int>(nextX) / CHIP_SIZE;
			int rightTile = static_cast<int>(nextX + kWidth - 1) / CHIP_SIZE;
			int topTile = static_cast<int>(m_pos.y) / CHIP_SIZE;
			int bottomTile = static_cast<int>(m_pos.y + kHeight - 1) / CHIP_SIZE;

			if (!map.IsWall(leftTile, topTile) &&
				!map.IsWall(rightTile, topTile) &&
				!map.IsWall(leftTile, bottomTile) &&
				!map.IsWall(rightTile, bottomTile))
			{
				m_pos.x = nextX;
			}
		}

		// Y軸方向の移動チェック
		if (m_vec.y != 0.0f)
		{
			float nextY = m_pos.y + m_vec.y * m_Speed;

			int leftTile = static_cast<int>(m_pos.x) / CHIP_SIZE;
			int rightTile = static_cast<int>(m_pos.x + kWidth - 1) / CHIP_SIZE;
			int topTile = static_cast<int>(nextY) / CHIP_SIZE;
			int bottomTile = static_cast<int>(nextY + kHeight - 1) / CHIP_SIZE;

			if (!map.IsWall(leftTile, topTile) &&
				!map.IsWall(rightTile, topTile) &&
				!map.IsWall(leftTile, bottomTile) &&
				!map.IsWall(rightTile, bottomTile))
			{
				m_pos.y = nextY;
			}
		}
	}

	// 画面外判定
	if (m_pos.y < 0.0f) { m_pos.y = 0.0f; }
	if (m_pos.x < 0.0f) { m_pos.x = 0.0f; }
	if (m_pos.y > Game::kScreenHeight - kHeight) { m_pos.y = Game::kScreenHeight - kHeight; }
	if (m_pos.x > Game::kScreenWidth - kWidth) { m_pos.x = Game::kScreenWidth - kWidth; }
}

void Player::Draw()
{
	int animNo = (m_animFrame % kWalkAnimTotalFrame) / kSingleAnimFrame;

	if (m_isDead)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
	}

	int srcX = animNo * kWidth;
	int srcY = static_cast<int>(m_dir) * kHeight;

	DrawRectGraph(
		static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		srcX, srcY,
		kWidth, kHeight,
		m_runHandle, true,
		false
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

#ifdef _DEBUG
	// 当たり判定用のデバッグ表示
	Vec2 center = GetColCenter();
	DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), static_cast<int>(GetColRadius()), GetColor(255, 0, 0), false);
#endif
}

Vec2 Player::GetColCenter() const
{
	return Vec2(m_pos.x + kWidth / 2.0f, m_pos.y + kHeight / 2.0f);
}

float Player::GetColRadius() const
{
	return kColRadius;
}

void Player::TakeDamage(int damage)
{
	if (m_isDead) return;

	m_hp -= damage;

	if (m_hp <= 0)
	{
		m_hp = 0;
		m_isDead = true;
	}
}

// ★追加：MP消費処理
bool Player::ConsumeMp(int cost)
{
	if (m_mp >= cost)
	{
		m_mp -= cost;
		return true; // 消費成功
	}
	return false; // MP不足
}

// ★追加：経験値獲得＆レベルアップ処理
bool Player::GainExp(int exp)
{
	m_exp += exp;

	// 必要経験値に達したらレベルアップ
	if (m_exp >= m_nextLevelExp)
	{
		m_level++;
		m_nextLevelExp += m_level * 30; // 次の必要経験値を上昇

		// ステータス上昇
		m_attack += 5;
		m_maxHp += 20;
		m_maxMp += 10;

		// 全回復
		FullHeal();

		return true; // レベルアップした！
	}

	return false; // レベルアップはしなかった
}

bool Player::IsDead() const
{
	return m_hp <= 0;
}

void Player::FullHeal()
{
	// HPとMPを最大値まで回復
	m_hp = m_maxHp;
	m_mp = m_maxMp;
	m_isDead = false;
}