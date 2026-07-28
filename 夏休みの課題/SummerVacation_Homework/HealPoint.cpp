#include "HealPoint.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"

namespace
{
	// グラフィックの切り取りサイズ（32x32px）
	constexpr int kWidth = 16;
	constexpr int kHeight = 32;

	// ステータス
	constexpr int kDefaultHP = 100;

	// アニメーション関連の定義
	constexpr int kHealAnimNum = 12;   // 12コマ

	//当たり判定の半径
	constexpr float kColRadius = 12.0f;

	constexpr int kSingleAnimFrame = 4;

	constexpr int kIdleAnimTotalFrame = kHealAnimNum * kSingleAnimFrame;
}

HealPoint::HealPoint():
	m_HealHandle(-1),
	m_animFrame(0)
{
}

HealPoint::~HealPoint()
{
}

void HealPoint::Init()
{
	m_pos.x = 500;
	m_pos.y = 400;

	m_animFrame = 0;

}

void HealPoint::End()
{
}

void HealPoint::Update(Player& player)
{
	m_animFrame++;

	Vec2 myCenter = GetColCenter();  // ヒールポイントの中心
	Vec2 playerCenter = player.GetColCenter();  // プレイヤーの中心

	// 2点間の距離の2乗を計算
	float dx = myCenter.x - playerCenter.x;
	float dy = myCenter.y - playerCenter.y;
	float distSq = dx * dx + dy * dy;

	// お互いの当たり判定の半径を合計
	float radiusSum = GetColRadius() + player.GetColRadius();

	// 距離が半径の合計以下なら全回復
	if (distSq <= radiusSum * radiusSum)
	{
		player.FullHeal();
	}

}

void HealPoint::Draw()
{
	//現在のフレーム数から表示したいコマ番号を計算で求める
	int animNo = (m_animFrame % kIdleAnimTotalFrame) / kSingleAnimFrame;

	DrawRectGraph(static_cast<int>(m_pos.x), static_cast<int>(m_pos.y),
		animNo * kWidth, 0, kWidth, kHeight,
		m_HealHandle, true);
#ifdef _DEBUG
	// 当たり判定用のデバッグ表示
	Vec2 center = GetColCenter();
	DrawCircle(static_cast<int>(center.x), static_cast<int>(center.y), static_cast<int>(GetColRadius()), GetColor(255, 0, 0), false);
#endif

}

Vec2 HealPoint::GetColCenter() const
{
	return Vec2(static_cast<int>(m_pos.x) + kWidth / 2.0f, static_cast<int>(m_pos.y) + kHeight / 2.0f);
}

float HealPoint::GetColRadius() const
{
	return kColRadius;
}
