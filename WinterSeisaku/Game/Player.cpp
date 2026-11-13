#include "Player.h"
#include <DxLib.h>
#include "../Application.h"
#include"../FileLoader.h"
#include"../Input.h"

// プレイヤー画像
constexpr int player_img_width = 32;
constexpr int player_img_height = 32;
constexpr float player_scale = 2.0f;

// アニメーション
constexpr int anim_interval = 8;
constexpr int walk_frame_count = 3;

// プレイヤーの向き(画像の段の番号)
constexpr int dir_down = 0;
constexpr int dir_left = 1;
constexpr int dir_right = 2;
constexpr int dir_up = 3;

// プレイヤーの攻撃アニメーション
constexpr int attack_start_col = 3; // 攻撃の画像が始まる列
constexpr int attack_frame_count = 3; // 攻撃のコマ数
constexpr int attack_interval = 5; // 何フレーム見せるか

constexpr float player_default_speed = 4.0f;

void Player::NormalUpdate()
{
}

void Player::ResurrectingUpdate()
{
}

Player::Player():
	pos_{ 640.0f, 360.0f },
	handle_(-1),
	playerSpeed(player_default_speed)
{
	handle_ = Application::GetInstance().GetLoader()->LoadGraph(L"Asset/img/player.png", AssetType::temporary);
	update_ = &Player::NormalUpdate;
}

void Player::Update(Input& input)
{
	Vector2 vel = {};
	if (input.IsPressed("up"))
	{
		vel.y = -1.0f;
	}

	if (input.IsPressed("down"))
	{
		vel.y = 1.0f;
	}

	if (input.IsPressed("left"))
	{
		vel.x = -1.0f;
	}

	if (input.IsPressed("right"))
	{
		vel.x = 1.0f;
	}

	bool moving = (vel.x != 0.0f || vel.y != 0.0f); // 動いているかの判定

	if (vel.x < 0.0f)
	{
		direction_ = dir_left;
	}
	else if (vel.x > 0.0f)
	{
		direction_ = dir_right;
	}
	else if (vel.y < 0.0f)
	{
		direction_ = dir_up;
	}
	else if (vel.y > 0.0f)
	{
		direction_ = dir_down;
	}


	vel.Normalize();
	vel *= playerSpeed;
	pos_ += vel;

	if (!isAttacking_ && input.IsTriggered("attack"))
	{
		isAttacking_ = true;
		attackTimer_ = 0;
	}

	if (isAttacking_)
	{
		const int weaponoffset = (static_cast<int>(weapontype_) - 1) * attack_frame_count;
		const int attackFrame = attackTimer_ / attack_interval;
		// 攻撃アニメーションの、何列目を表示するか
		animIndex_ = attack_start_col + weaponoffset + attackFrame;

		if (++attackTimer_ >= attack_frame_count * attack_interval)
		{
			isAttacking_ = false;
		}
	}
	else if (moving)
	{
		if (++animTimer_ >= anim_interval)
		{
			animTimer_ = 0;
			animIndex_ = (animIndex_ + 1) % walk_frame_count;
		}
	}
	else
	{
		animTimer_ = 0;
		animIndex_ = 0;
	}

	Update();
}

void Player::Update()
{
	(this->*update_)();
}

void Player::Draw()
{
	DrawRectRotaGraph(static_cast<int>(pos_.x), static_cast<int>(pos_.y),
		player_img_width * animIndex_, // 何コマ目か
		player_img_height * direction_, // 何段目か
		player_img_width, player_img_height,
		player_scale, 0.0f,
		handle_, true);
}
