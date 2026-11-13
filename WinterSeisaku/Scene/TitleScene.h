#pragma once
#include "SceneBase.h"

class TitleScene : public SceneBase
{
private:
	int backH_;
	int frame_ = 0;
	int m_frameCount; 

	using UpdateFunc_t = void(TitleScene::*)(Input&);
	using DrawFunc_t = void(TitleScene::*)();

	UpdateFunc_t update_; // Update関数用メンバ関数ポインタ
	DrawFunc_t draw_;     // Draw関数用メンバ関数ポインタ

	// Updateに使う関数
	void InitUpdate(Input&);
	void FadeInUpdate(Input&); // フェードイン中
	void NormalUpdate(Input& input); // 通常(入力受付)
	void FadeOutUpdate(Input&); // フェードアウト中

	// Drawに使う関数
	void FadeDraw();   // フェード中
	void NormalDraw(); // 通常
	void EmptyDraw();

public:
	TitleScene(SceneController& ctrl);
	~TitleScene();
	void Update(Input& input) override;
	void Draw();
};

