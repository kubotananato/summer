#pragma once
#include"../Geometry.h"

enum class WeaponType
{
	none,
	knife, // 小太刀
	sword, // 大剣
	staff, // 杖
	spear, // 長槍

};

class Input;
class Player
{
private:
	WeaponType weapontype_ = WeaponType::knife;
	Position2 pos_;
	int handle_; // プレイヤーの画像
	int animIndex_ = 0; // 何コマ目か
	int animTimer_ = 0; // コマを切り替える時間 
	int direction_ = 0; // 何段目か
	float playerSpeed = 0.0f;

	bool isAttacking_ = false;
	int attackTimer_ = 0;

	using Update_t = void (Player::*)();
	Update_t update_;

	void NormalUpdate();
	void ResurrectingUpdate();

public:
	Player();
	void Update(Input& input);
//	void ClampPlayerPosition(Position2& pos);
	void Update();
	void Draw();
};

