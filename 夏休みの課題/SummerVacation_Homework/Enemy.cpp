#include "Enemy.h"
#include "DxLib.h"
#include "Game.h"

namespace
{
	constexpr int kDefaultHP = 100;
}

Enemy::Enemy():
	m_graphHandle(-1),
	m_hp(0),
	m_Attack(0)
{
}

Enemy::~Enemy()
{
}

void Enemy::Init()
{
	m_hp = kDefaultHP;
}

void Enemy::End()
{
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
}
