#pragma once
struct Vector2
{
	float x = 0.0f;
	float y = 0.0f;

	/// <summary>
	/// 正規化する
	/// </summary>
	void Normalize();

	float SqLength() const;
	
	/// <summary>
	/// ベクトルの大きさを返す
	/// </summary>
	/// <returns></returns>
	float Length() const;

	Vector2 Normalized() const;

	//右辺値がある-演算と(ただの引き算)
	Vector2 operator-(const Vector2& right)const;

	//右辺値がない-演算(ベクトルをマイナス化する)
	Vector2 operator-()const;

	//ベクトルをn倍する(ベクトルのスケーリング)
	Vector2 operator*(float scale)const;
	//ベクトルを割り算する
//	Vector2 operator/(float div)const;

	void operator+=(const Vector2& vec);
	void operator-=(const Vector2& vec);
	void operator*=(float scale);
//	void operator/=(float div);

};

Vector2 operator+(const Vector2& left, const Vector2& right);

using Position2 = Vector2;

struct Size
{
	int w;
	int h;
};

float DegreeToRadian(float degree);

// 円
struct Circle
{
	Position2 pos; // 中心座標
	float radius; // 半径
};

// 短形(長方形)
struct Rect
{

};