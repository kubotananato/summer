#include "Input.h"
#include <DxLib.h>

Input::Input()
{
	ResetInputSetting();
	for (const auto& row : inputTable_)
	{
		currentInput_[row.first] = false;
		lastInput_[row.first] = false;
	}
}

void Input::ResetInputSetting()
{
	inputTable_["ok"] = {
	{ InputDevice::keyboard,KEY_INPUT_RETURN },
	{ InputDevice::pad,PAD_INPUT_A },
	};
	inputTable_["back"] = {
		{ InputDevice::keyboard,KEY_INPUT_SPACE },
		{ InputDevice::pad,PAD_INPUT_B },
	};

	inputTable_["attack"] = {
		{ InputDevice::keyboard,KEY_INPUT_Z },
		{ InputDevice::pad,PAD_INPUT_C },
	};

	inputTable_["up"] = {
		{ InputDevice::keyboard,KEY_INPUT_UP },
		{ InputDevice::keyboard,KEY_INPUT_W },
		{ InputDevice::pad,PAD_INPUT_UP }
	};
	inputTable_["down"] = {
		{ InputDevice::keyboard,KEY_INPUT_DOWN },
		{ InputDevice::keyboard,KEY_INPUT_S },
		{ InputDevice::pad,PAD_INPUT_DOWN }
	};
	inputTable_["right"] = {
		{ InputDevice::keyboard,KEY_INPUT_RIGHT },
		{ InputDevice::keyboard,KEY_INPUT_D },
		{ InputDevice::pad,PAD_INPUT_RIGHT }
	};
	inputTable_["left"] = {
		{ InputDevice::keyboard,KEY_INPUT_LEFT },
		{ InputDevice::keyboard,KEY_INPUT_A },
		{ InputDevice::pad,PAD_INPUT_LEFT }
	};
}

bool Input::IsPressed(const std::string& code) const
{
	if (!currentInput_.contains(code))
	{
		return false;
	}
	return currentInput_.at(code);
}

bool Input::IsTriggered(const std::string& code) const
{
	if (!currentInput_.contains(code))
	{
		return false;
	}
	return currentInput_.at(code) && !lastInput_.at(code);
}

void Input::Update()
{
	lastInput_ = currentInput_;
	std::array<char, 256> keyState;

	GetHitKeyStateAll(keyState.data());
	auto padNum = GetJoypadNum();
	auto padState = 0;
	if (padNum > 0)
	{
		padState = GetJoypadInputState(DX_INPUT_PAD1);
	}

	for (const auto& row : inputTable_)
	{
		for (const auto& inputInfo : row.second)
		{
			switch (inputInfo.type)
			{
			case InputDevice::keyboard:
				currentInput_[row.first] = keyState[inputInfo.id];
				break;
			case InputDevice::pad:
				currentInput_[row.first] = (padState & inputInfo.id);
				break;
			}
			if (currentInput_[row.first])
			{
				break;
			}
		}
	}

}
