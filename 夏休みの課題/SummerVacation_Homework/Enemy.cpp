#include "Enemy.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	// グラフィックの切り取りサイズ（32x32px）
	constexpr int kWidth = 32;
	constexpr int kHeight = 32;

	// ステータス
	constexpr int kDefaultHP = 100;

	// アニメーション関連の定義
	constexpr int kIdleAnimNum = 6;     // 6コマ
	constexpr int kRunAnimNum = 12;

	//当たり判定の半径
	constexpr float kColRadius = 12.0f;

	constexpr int kSingleAnimFrame = 4;

	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;
}

Enemy::Enemy():
	m_IdleHandle(-1),
	m_RunHandle(-1),
	m_hp(0),
	m_Attack(0), 
	m_animFrame(0),
	m_isFlip(false),
	m_isMoving(false),
	m_isDead(false)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	m_pos.x = Game::kScreenWidth/2;
	m_pos.y = Game::kScreenHeight / 2;

	m_vec.x = 0.0f;
	m_vec.y = 0.0f;

	m_animFrame = 0;
	m_isDead = false;

	m_hp = kDefaultHP;
}

void Enemy::End()
{
}

void Enemy::Update()
{
	m_animFrame++;
}

void Enemy::Draw()
{

	//移動中かどうかでアニメーションを変更する
	int tempTotalFrame = kIdleAnimTotalFrame;
	int tempHanndle = m_IdleHandle;
	if (m_isMoving)
	{
		tempTotalFrame = kRunAnimTotalFrame;
		tempHanndle = m_RunHandle;
	}

	//現在のフレーム数から表示したいコマ番号を計算で求める
	int animNo = (m_animFrame % kIdleAnimTotalFrame) / kSingleAnimFrame;

	if (m_isDead)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 0);
	}

	int currentHandle = m_IdleHandle; // デフォルトは待機用
	if (m_isMoving)
	{
		currentHandle = m_RunHandle;  // 移動中は走り用
	}
	DrawRectGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		animNo * kWidth, 0,
		kWidth, kHeight,
		currentHandle, true, 
		m_isFlip);

#ifdef _DEBUG
	// 当たり判定用のデバッグ表示
	Vec2 center = GetColCenter();
	DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), static_cast<int>(GetColRadius()), GetColor(255, 0, 0), false);
#endif
}

Vec2 Enemy::GetColCenter() const
{
	return Vec2(m_pos.x + kWidth / 2.0f, m_pos.y + kHeight / 2.0f);
}

float Enemy::GetColRadius() const
{
	return kColRadius;
}