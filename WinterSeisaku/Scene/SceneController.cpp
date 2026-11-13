#include "SceneController.h"
#include "SceneBase.h"

void SceneController::ResetScene(std::shared_ptr<SceneBase> newScene)
{
	scenes_.clear();
	scenes_.push_back(newScene);
}

void SceneController::ChangeScene(std::shared_ptr<SceneBase> newScene)
{
	if (scenes_.empty())//empty()はリストやvectorが空っぽであることを示します
	{
		scenes_.push_back(newScene);
		return;
	}
	scenes_.back() = newScene;//現在使用してるシーンを入れ替える
}

void SceneController::PushScene(std::shared_ptr<SceneBase> newScene)
{
	scenes_.push_back(newScene);
}

void SceneController::PopScene()
{
	if (scenes_.size() <= 1)
	{
		return;
	}
	// push_backされたシーンを削除する
	scenes_.pop_back();
}

void SceneController::Update(Input& input)
{
	// 今一番手前にあるシーンのUpdateを呼び出す
	scenes_.back()->Update(input);
}

void SceneController::Draw()
{
	// すべて描画する
	for (auto& scene : scenes_)
	{
		scene->Draw();
	}
}
