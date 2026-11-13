#include "Geometry.h"
#include <cmath>
#include <cassert>
#include <DxLib.h>

void Vector2::Normalize()
{
	float Sqlen = SqLength();
	// エラー処理
	if (Sqlen == 0.0f)
	{
		x = 0.0f;
		y = 0.0f;
		return;
	}
	float len = Length();
	assert(len != 0.0f);
	x /= len;
	y /= len;
}

float Vector2::SqLength() const
{
	return (x * x) + (y * y);
}

float Vector2::Length() const
{
	return sqrtf(SqLength()); // ベクトルの大きさを返す
}

Vector2 Vector2::Normalized() const
{
	float len = Length();
	if (len == 0.0f)
	{
		return{};
	}
	return { x / len, y / len };
}

Vector2 Vector2::operator-(const Vector2& right) const
{
	return { x - right.x,y - right.y };
}

Vector2 Vector2::operator-() const
{
	return { -x, - y };
}

Vector2 Vector2::operator*(float scale) const
{
	return { x * scale,y * scale };
}

void Vector2::operator+=(const Vector2& vec)
{
	x += vec.x;
	y += vec.y;
}

void Vector2::operator-=(const Vector2& vec)
{
	x -= vec.x;
	y -= vec.y;
}

void Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
}

Vector2 operator+(const Vector2& left, const Vector2& right)
{
	return { left.x + right.x, left.y + right.y };
}

float DegreeToRadian(float degree)
{
	return (degree * DX_PI_F) / 180.0f;
}
