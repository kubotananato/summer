#pragma once

class SceneTitle
{
public:
	SceneTitle();
	~SceneTitle();

	void Init();
	void End();
	void Update();
	void Draw();

	bool isFinished = false;

private:
	int m_oldPad;       // 前フレームのパッド入力
	bool m_oldSpace = false; // 前フレームのキーボード入力
	int m_blinkTimer;
};

