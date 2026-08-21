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

	// ★ 敵ごとのアニメーションと徘徊タイマーをバラバラにずらして同期を防ぐ
	m_animFrame = std::rand() % 60;
	m_wanderTimer = std::rand() % 60;

	m_isDead = false;
	m_isMoving = false;
	m_state = State::Wander;

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

	// 4. 移動処理の反映
	m_pos.x += m_vec.x;
	m_pos.y += m_vec.y;

	// ★ 【追加】画面外に出てしまわないように座標を画面内に制限（クランプ）する
	if (m_pos.x < 0.0f) m_pos.x = 0.0f;
	if (m_pos.y < 0.0f) m_pos.y = 0.0f;
	if (m_pos.x > Game::kScreenWidth - kWidth)  m_pos.x = static_cast<float>(Game::kScreenWidth - kWidth);
	if (m_pos.y > Game::kScreenHeight - kHeight) m_pos.y = static_cast<float>(Game::kScreenHeight - kHeight);

	// 移動中フラグの更新
	m_isMoving = (m_vec.x != 0.0f || m_vec.y != 0.0f);

	// 左を向いていたら画像を反転
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

	// 敵の正向ベクトル(m_dir)との内積を計算
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
	// ★ 移動中で、かつ走り用ハンドルが正しく読み込めている場合のみ走りアニメーションを使う
	bool canUseRun = m_isMoving && (m_RunHandle != -1);

	int tempTotalFrame = canUseRun ? kRunAnimTotalFrame : kIdleAnimTotalFrame;
	int currentHandle = canUseRun ? m_RunHandle : m_IdleHandle;

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
	Vec2 center = GetColCenter();

	// 1. 敵の向いている角度（ラジアン）と視野角（半角）を計算
	float facingAngle = std::atan2(m_dir.y, m_dir.x);
	float halfFov = std::acos(kViewCos);

	// 2. 視界の左端・右端の角度
	float leftAngle = facingAngle - halfFov;
	float rightAngle = facingAngle + halfFov;

	// 3. 視界の端点座標を計算
	int leftX = static_cast<int>(center.x + std::cos(leftAngle) * kSearchRadius);
	int leftY = static_cast<int>(center.y + std::sin(leftAngle) * kSearchRadius);
	int rightX = static_cast<int>(center.x + std::cos(rightAngle) * kSearchRadius);
	int rightY = static_cast<int>(center.y + std::sin(rightAngle) * kSearchRadius);

	// 8. 当たり判定の円（赤線）
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