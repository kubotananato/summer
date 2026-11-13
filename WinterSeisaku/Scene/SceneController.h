#pragma once
#include <memory>
#include <list>

class SceneBase;
class Input;

class SceneController
{
private:
	std::list<std::shared_ptr<SceneBase>> scenes_;

public:
	// 今積まれているシーンを全部破棄し、新しいシーン1つだけの状態にする
	void ResetScene(std::shared_ptr<SceneBase> newScene);

	// シーンの切り替えを行う。
	void ChangeScene(std::shared_ptr<SceneBase> newScene);

	// 新しいシーンを押し込む
	void PushScene(std::shared_ptr<SceneBase> newScene);

	// 最後のシーンを外す
	void PopScene();

	void Update(Input& input);
	void Draw();
};

