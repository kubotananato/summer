#pragma once
#include "SceneBase.h"
class GameOverScene : public SceneBase
{
private:
	int handle_;
	int frame_;
	int frameCount_;

	using UpdateFunc_t = void (GameOverScene::*)(Input&);
	using DrawFunc_t = void (GameOverScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void InitUpdate(Input&);
	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);

	void FadeDraw();
	void NormalDraw();
	void EmptyDraw();

public:
	GameOverScene(SceneController& ctrl);
	~GameOverScene();
	void Update(Input& input) override;
	void Draw() override;
};

