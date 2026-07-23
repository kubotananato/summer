#include "Player.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	// グラフィックの切り取りサイズ（32x32px）
	constexpr int kWidth = 32;
	constexpr int kHeight = 32;

	// 初期Status
	constexpr int kDefaultHP = 100;
	constexpr int kDefaultMP = 100;
	constexpr int kDefaultAttack = 100;
	constexpr int kSpeed = 4;

	// アニメーション関連の定義
	constexpr int kIdleAnimNum = 6;     // 6コマ
	constexpr int kRunAnimNum = 12;

	constexpr int kSingleAnimFrame = 4;

	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;
}

Player::Player() :
	m_idleHandle(-1),
	m_animFrame(0),
	m_isFlip(false),
	m_angle(0.0f),
	m_isLanding(0),
	m_attack(0),
	m_hp(0),
	m_mp(0),
	m_maxMp(0),
	m_Speed(0),
	m_isDead(false),
	m_dir(Direction::Right)
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
	m_dir = Direction::Right;

	m_hp = kDefaultHP;
	m_maxMp = kDefaultMP;
	m_attack = kDefaultAttack;
	m_Speed = kSpeed;
}

void Player::End()
{
}

void Player::Update()
{
	m_animFrame++;

	int padInput = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	int speed = 4;

	if (padInput & PAD_INPUT_LEFT)
	{
		m_pos.x -= speed;
		m_dir = Direction::Left;
	}
	if (padInput & PAD_INPUT_RIGHT)
	{
		m_pos.x += speed;
		m_dir = Direction::Right;
	}
	if (padInput & PAD_INPUT_UP)
	{
		m_pos.y -= speed;
//		m_dir = Direction::Up;
	}
	if (padInput & PAD_INPUT_DOWN)
	{
		m_pos.y += speed;
//		m_dir = Direction::Down;
	}

	// 画面外判定
	if (m_pos.y < 0.0f) { m_pos.y = 0.0f; }
	if (m_pos.x < 0.0f) { m_pos.x = 0.0f; }
	if (m_pos.y > Game::kScreenHeight - kHeight) { m_pos.y = Game::kScreenHeight - kHeight; }
	if (m_pos.x > Game::kScreenWidth - kWidth) { m_pos.x = Game::kScreenWidth - kWidth; }
}

void Player::Draw()
{
	int animNo = (m_animFrame % kIdleAnimTotalFrame) / kSingleAnimFrame;

	if (m_isDead)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 255);
	}

	bool isFlip = (m_dir == Direction::Left);

	DrawRectGraph(
		(int)m_pos.x, (int)m_pos.y,
		animNo * kWidth, 0, // X座標: 0, 32, 64, 96, 128, 160 / Y座標: 0
		kWidth, kHeight,
		m_idleHandle, true,
		isFlip
	);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

bool Player::IsDead() const
{
	return m_hp <= 0;
}