#include "Enemy.h"
#include "DxLib.h"
#include "Game.h"
#include <cmath>
#include <cstdlib>

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

	// 当たり判定の半径
	constexpr float kColRadius = 12.0f;

	constexpr int kSingleAnimFrame = 4;

	constexpr int kIdleAnimTotalFrame = kIdleAnimNum * kSingleAnimFrame;
	constexpr int kRunAnimTotalFrame = kRunAnimNum * kSingleAnimFrame;

	// --- AIパラメータ ---
	constexpr float kWanderSpeed = 1.0f;     // 徘徊時の移動速度
	constexpr float kChaseSpeed = 2.0f;      // 追尾時の移動速度
	constexpr float kSearchRadius = 180.0f;  // 視界の届く距離
	constexpr float kLoseRadius = 250.0f;    // 諦める距離
	constexpr float kViewCos = 0.707f;       // 視野角（cos 45° ≒ 前方±45度、全体で90度）
}

Enemy::Enemy() :
	m_IdleHandle(-1),
	m_RunHandle(-1),
	m_hp(0),
	m_Attack(0),
	m_animFrame(0),
	m_isFlip(false),
	m_isMoving(false),
	m_isDead(false),
	m_state(State::Wander),
	m_wanderTimer(0),
	m_dir(1.0f, 0.0f)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	m_pos.x = Game::kScreenWidth / 2.0f;
	m_pos.y = Game::kScreenHeight / 2.0f;

	m_lastPos = m_pos;
	m_vec.x = 0.0f;
	m_vec.y = 0.0f;

	m_dir.x = 1.0f;
	m_dir.y = 0.0f;

	m_animFrame = 0;
	m_isDead = false;
	m_isMoving = false;
	m_state = State::Wander;
	m_wanderTimer = 0;

	m_hp = kDefaultHP;
}

void Enemy::End()
{
}

void Enemy::Update(const Vec2& playerPos)
{
	m_animFrame++;
	m_lastPos = m_pos;

	if (m_isDead) return;

	// 1. プレイヤーへのベクトルと距離の計算
	Vec2 toPlayer = Vec2(playerPos.x - m_pos.x, playerPos.y - m_pos.y);
	float dist = std::sqrt(toPlayer.x * toPlayer.x + toPlayer.y * toPlayer.y);

	// 2. 状態（State）の遷移判定
	if (m_state == State::Wander)
	{
		// 視界範囲内にプレイヤーが入ったら追尾開始
		if (CanSeePlayer(toPlayer, dist))
		{
			m_state = State::Chase;
		}
	}
	else if (m_state == State::Chase)
	{
		// 一定距離以上離れたら見失って徘徊に戻る
		if (dist > kLoseRadius)
		{
			m_state = State::Wander;
			m_wanderTimer = 0;
		}
	}

	// 3. 状態ごとの移動処理
	if (m_state == State::Wander)
	{
		UpdateWander();
	}
	else
	{
		UpdateChase(toPlayer, dist);
	}

	// 4. 移動処理の反映と向き（反転・アニメーション）の決定
	m_pos.x += m_vec.x;
	m_pos.y += m_vec.y;

	// 移動中フラグの更新
	m_isMoving = (m_vec.x != 0.0f || m_vec.y != 0.0f);

	// 左を向いていたら画像を反転（描画の仕様に合わせて調整）
	if (m_dir.x < -0.1f)
	{
		m_isFlip = true;
	}
	else if (m_dir.x > 0.1f)
	{
		m_isFlip = false;
	}
}

// 視界判定（距離 ＋ 前方の角度）
bool Enemy::CanSeePlayer(const Vec2& toPlayer, float dist) const
{
	if (dist > kSearchRadius || dist <= 0.001f) return false;

	// プレイヤー方向の単位ベクトル
	Vec2 targetDir = Vec2(toPlayer.x / dist, toPlayer.y / dist);

	// 敵の正面向ベクトル(m_dir)との内積を計算
	float dot = m_dir.x * targetDir.x + m_dir.y * targetDir.y;

	// 内積結果が指定角度（cos45° ≒ 0.707）以上なら視界内
	return (dot >= kViewCos);
}

// 徘徊処理
void Enemy::UpdateWander()
{
	m_wanderTimer--;

	// タイマー消化時に方向を変更、または一定確率で立ち止まる
	if (m_wanderTimer <= 0)
	{
		m_wanderTimer = 60 + (std::rand() % 90); // 1〜2.5秒間行動を維持

		if (std::rand() % 3 == 0)
		{
			// 1/3の確率で停止
			m_vec = Vec2(0.0f, 0.0f);
		}
		else
		{
			// ランダムな角度を決めて進む
			float angle = static_cast<float>(std::rand() % 360) * 3.14159f / 180.0f;
			m_dir.x = std::cos(angle);
			m_dir.y = std::sin(angle);

			m_vec.x = m_dir.x * kWanderSpeed;
			m_vec.y = m_dir.y * kWanderSpeed;
		}
	}
}

// 追尾処理
void Enemy::UpdateChase(const Vec2& toPlayer, float dist)
{
	if (dist > 0.001f)
	{
		// プレイヤーの方向へ移動
		m_dir.x = toPlayer.x / dist;
		m_dir.y = toPlayer.y / dist;

		m_vec.x = m_dir.x * kChaseSpeed;
		m_vec.y = m_dir.y * kChaseSpeed;
	}
}

void Enemy::Draw()
{
	// 移動中かどうかでアニメーションを変更する
	int tempTotalFrame = m_isMoving ? kRunAnimTotalFrame : kIdleAnimTotalFrame;
	int currentHandle = m_isMoving ? m_RunHandle : m_IdleHandle;

	// 現在のフレーム数から表示したいコマ番号を計算
	int animNo = (m_animFrame % tempTotalFrame) / kSingleAnimFrame;

	if (m_isDead)
	{
		SetDrawBlendMode(DX_BLENDMODE_INVSRC, 0);
	}

	DrawRectGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		animNo * kWidth, 0,
		kWidth, kHeight,
		currentHandle, true,
		m_isFlip);

	if (m_isDead)
	{
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

#ifdef _DEBUG
	// 当たり判定用のデバッグ表示
	Vec2 center = GetColCenter();
	DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), static_cast<int>(GetColRadius()), GetColor(255, 0, 0), false);

	// 敵の正面方向ラインを表示（赤色）
	DrawLine(static_cast<int>(center.x), static_cast<int>(center.y),
		static_cast<int>(center.x + m_dir.x * 30.0f), static_cast<int>(center.y + m_dir.y * 30.0f),
		GetColor(255, 0, 0));

	// 追尾状態なら黄色い円を表示
	if (m_state == State::Chase)
	{
		DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), 15, GetColor(255, 255, 0), false);
	}
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