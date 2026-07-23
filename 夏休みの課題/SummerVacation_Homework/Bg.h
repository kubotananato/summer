#pragma once
class Bg
{
public:
	Bg();
	~Bg();

	void Init();
	void End();
	void Update();
	void Draw();

	// 使用するグラフィックの設定
	void SetHandle(int handle) { m_graphHandle = handle; }

private:
	int m_graphHandle;

};


