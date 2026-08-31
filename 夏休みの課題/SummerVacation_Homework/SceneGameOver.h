#pragma once

class SceneGameOver
{
public:
	SceneGameOver();
	~SceneGameOver();

	void Init();
	void End();
	void Update();
	void Draw();

	bool isFinished = false;

private:
	int m_oldPad;
	bool m_oldSpace;
	int m_fontHandle;
};