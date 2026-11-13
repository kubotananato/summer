/*
#pragma once
#include <cmath>
#include <cassert>

class Vec2
{
public:
	float x = 0.0f;
	float y = 0.0f;

public:
	// ベクトルの長さの2乗を取得する
	float SqLength() const
	{
		return (x * x) + (y * y);
	}

	// ベクトルの長さを取得する
	float Length() const
	{
		return sqrtf(SqLength());
	}

	// 自身の正規化を行う
	Vec2 Normalize()
	{
		// 長さの2乗を出す
		float lenSq = SqLength();

		// エラー処理
		// 0以下ならば、警告出るようにする(リリース時クラッシュ回避)
		if (lenSq == 0.0f)
		{
			assert(lenSq != 0 && "Vector2::Normalize : 0 division"); // Debugビルドで0除算を検出
			return { 0,0};
		}
		// ベクトルの大きさを出す
		float len = Length();
		// 成分を大きさで割る
		return { x / len,y / len };
	}
};
*/