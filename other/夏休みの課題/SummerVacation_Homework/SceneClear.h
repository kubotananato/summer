#pragma once

class SceneClear
{
public:
	SceneClear();
	~SceneClear();

	void Init();
	void Update();
	void End();
	void Draw();

	bool isFinished = false;

private:
	int m_oldPad;
	bool m_oldSpace;
	int m_fontHandle;
};