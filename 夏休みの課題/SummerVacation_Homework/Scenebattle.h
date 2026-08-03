#pragma once
class Scenebattle
{
public:
	Scenebattle();
	~Scenebattle();

	void Init();
	void End();
	void Update();
	void Draw();

	bool isFinished = false;
};

