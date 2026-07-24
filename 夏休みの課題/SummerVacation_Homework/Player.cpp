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

	//当たり判定の半径
	constexpr float kColRadius = 12.0f;

	constexpr int kSingleAnimFrame = 4;

	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;
}

Player::Player() :
	m_idleHandle(-1),
	m_runHandle(-1),
	m_animFrame(0),
	m_isFlip(false),
	m_isMoving(false),
	m_angle(0.0f),
	m_isLanding(0),
	m_attack(0),
	m_hp(0),
	m_mp(0),
	m_maxMp(0),
	m_Speed(0),
	m_isDead(false)
//	m_dir(Direction::Right)
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
//	m_dir = Direction::Right;

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



	// コントローラーでキャラクターを移動させる
	if (!m_isDead)
	{
		int pad = GetJoypadInputState(DX_INPUT_KEY_PAD1);

		m_vec.x = 0.0f;
		m_vec.y = 0.0f;
		m_isMoving = false;

		// else if で繋ぐことで、同時にボタンが押されても1方向しか動かなくする！
		if (pad & PAD_INPUT_UP)
		{
			m_vec.y = -1.0f;
			m_isMoving = true;
		}
		else if (pad & PAD_INPUT_DOWN)
		{
			m_vec.y = 1.0f;
			m_isMoving = true;
		}
		else if (pad & PAD_INPUT_LEFT)
		{
			m_vec.x = -1.0f;
			m_isMoving = true;
			m_isFlip = true;
		}
		else if (pad & PAD_INPUT_RIGHT)
		{
			m_vec.x = 1.0f;
			m_isMoving = true;
			m_isFlip = false;
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
	//移動中かどうかでアニメーションを変更する
	int tempTotalFrame = kIdleAnimTotalFrame;
	int tempHanndle = m_idleHandle;
	if (m_isMoving)
	{
		tempTotalFrame = kRunAnimTotalFrame;
		tempHanndle = m_runHandle;
	}

	//現在のフレーム数から表示したいコマ番号を計算で求める
	int animNo = (m_animFrame % kIdleAnimTotalFrame) / kSingleAnimFrame;

	if (m_isDead)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 0);
	}


	int currentHandle = m_idleHandle; // デフォルトは待機用
	if (m_isMoving)
	{
		currentHandle = m_runHandle;  // 移動中は走り用
	}

	DrawRectGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		animNo * kWidth, 0,
		kWidth, kHeight,
		currentHandle, true, 
		m_isFlip);

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