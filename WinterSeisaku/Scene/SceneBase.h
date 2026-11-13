#pragma once

class SceneController;
class Input;

/// <summary>
/// シーンの基底クラス
/// </summary>
class SceneBase
{
protected:
	SceneController& controller_; // シーンコントローラの参照

public:
	SceneBase(SceneController& ctrl);
	virtual ~SceneBase() = default;
	virtual void Update(Input& input) = 0;
	virtual void Draw() = 0;
};

