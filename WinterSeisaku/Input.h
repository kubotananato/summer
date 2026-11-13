#pragma once
#include<array>
#include<string>
#include<map>
#include<vector>

/// <summary>
/// 入力機器
/// </summary>
enum class InputDevice
{
	keyboard,
	pad,
	mouse
};

// 実入力情報
struct InputInfo
{
	InputDevice type; // 入力機器種別
	int id; // 入力ID
};

class Input
{
private:
	using InputTable_t = std::map < std::string, std::vector<InputInfo>>;
	InputTable_t inputTable_; // 入力テーブル

	using InputState_t = std::map < std::string, bool>;//キー文字列と、押したか押してないかbool
	InputState_t currentInput_; //　現在押されている状況
	InputState_t lastInput_; // 最後に押されている状況

/// <summary>
/// 入力テーブルをリセットする
/// </summary>
	void ResetInputSetting();

public:
	Input();

	bool IsPressed(const std::string& code)const;  // 押している状態か
	bool IsTriggered(const std::string& code)const; // 押した瞬間か

	void Update();

};

