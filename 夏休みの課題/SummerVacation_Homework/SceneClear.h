#pragma once

class SceneClear
{
public:
	SceneClear();
	~SceneClear();

	void Init();
	void Update();
	void Draw();

	bool isFinished = false;

private:
	int m_oldPad;
	bool m_oldSpace;
};