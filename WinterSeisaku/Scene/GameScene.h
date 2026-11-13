#pragma once
#include"SceneBase.h"
#include"../Geometry.h"
#include<memory>
#include<array>
class Player;

class GameScene : public SceneBase
{
private:
	std::shared_ptr<Player> player_;

	int handle_;
	int frame_;
	int frameCount_;

	using UpdateFunc_t = void (GameScene::*)(Input&);
	using DrawFunc_t = void (GameScene::*)();

	UpdateFunc_t update_;
	DrawFunc_t draw_;

	void InitUpdate(Input&);
	void FadeInUpdate(Input&);
	void NormalUpdate(Input& input);
	void FadeOutUpdate(Input&);

	void NormalDraw();
	void FadeDraw();
	void EmptyDraw();

public:
	GameScene(SceneController& ctrl);
	~GameScene();
	void Update(Input& input) override;
	void Draw() override;
};

