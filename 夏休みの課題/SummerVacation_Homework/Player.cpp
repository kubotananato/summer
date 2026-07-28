#include "Player.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	// グラフィックの切り取り
	constexpr int kWidth = 32;
	constexpr int kHeight = 32;

	// 初期Status
	constexpr int kDefaultHP = 100;
	constexpr int kDefaultMP = 100;
	constexpr int kDefaultAttack = 100;
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
	m_isLanding(0),
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

	m_maxHp = kDefaultHP;
	m_maxMp = kDefaultMP;
	m_attack = kDefaultAttack;
	m_Speed = kSpeed;
}

void Player::End()
{
}

void Player::Update()
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
			m_dir = Direction::Left;   //左向き
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

		// 移動方向にスピードを掛けて移動
		m_pos.x += m_vec.x * m_Speed;
		m_pos.y += m_vec.y * m_Speed;
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

bool Player::IsDead() const
{
	return m_hp <= 0;
}

void Player::FullHeal()
{
	// HPとMPを最大値（初期値）まで回復
	m_hp = m_maxHp;
	m_mp = m_maxMp; 
}
