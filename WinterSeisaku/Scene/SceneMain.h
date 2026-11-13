#pragma once
#include "SceneBase.h"

class SceneMain : public SceneBase
{
public:
	SceneMain(SceneController& ctrl);
	~SceneMain();

	void Init();
	void End();

	void Update(Input& input) override;
	void Draw() override;



private:
	int m_frameCount;

};

