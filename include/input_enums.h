#pragma once

typedef enum gamepad_button {
	gp_A,
	gp_B,
	gp_Z,
	gp_start,
	gp_up,
	gp_down,
	gp_left,
	gp_right,
	gp_L,
	gp_R,
	gp_C_up,
	gp_C_down,
	gp_C_left,
	gp_C_right,
	gp_analog_horizontal,
	gp_analog_vertical
} gamepad_button;

typedef enum gamepad_button_state {
	gp_released = 0,
	gp_pressed,
    gp_held,
} gamepad_button_state;

/*
inline gamepad_button parse_gamepad_button(const char* button)
{
	if (state == "FaceDown")
		return gamepad_button::kFaceDown;
	if (state == "FaceLeft")
		return gamepad_button::kFaceLeft;
	if (state == "FaceUp")
		return gamepad_button::kFaceUp;
	if (state == "FaceRight")
		return GamePadButton::kFaceRight;
	if (state == "DPadDown")
		return GamePadButton::kDPadDown;
	if (state == "DPadLeft")
		return GamePadButton::kDPadLeft;
	if (state == "DPadUp")
		return GamePadButton::kDPadUp;
	if (state == "DPadRight")
		return GamePadButton::kDPadRight;
	if (state == "LeftStickDown")
		return GamePadButton::kLeftStickDown;
	if (state == "LeftStickLeft")
		return GamePadButton::kLeftStickLeft;
	if (state == "LeftStickUp")
		return GamePadButton::kLeftStickUp;
	if (state == "LeftStickRight")
		return GamePadButton::kLeftStickRight;
	if (state == "RightStickDown")
		return GamePadButton::kRightStickDown;
	if (state == "RightStickLeft")
		return GamePadButton::kRightStickLeft;
	if (state == "RightStickUp")
		return GamePadButton::kRightStickUp;
	if (state == "RightStickRight")
		return GamePadButton::kRightStickRight;
	if (state == "Select")
		return GamePadButton::kSelect;
	if (state == "Start")
		return GamePadButton::kStart;
	if (state == "LeftShoulder")
		return GamePadButton::kLeftShoulder;
	if (state == "RightShoulder")
		return GamePadButton::kRightShoulder;
	if (state == "LeftTrigger")
		return GamePadButton::kLeftTrigger;
	if (state == "RightTrigger")
		return GamePadButton::kRightTrigger;
	if (state == "LeftStick")
		return GamePadButton::kLeftStick;

	return GamePadButton::kRightStick;
}

inline GamePadAnalog ParseGamePadAnalogState(const char* state)
{
	if (state == "LeftStickHorizontal")
		return GamePadAnalog::kLeftStickHorizontal;
	if (state == "LeftStickVertical")
		return GamePadAnalog::kLeftStickVertical;
	if (state == "RightStickHorizontal")
		return GamePadAnalog::kRightStickHorizontal;
	if (state == "RightStickVertical")
		return GamePadAnalog::kRightStickVertical;
	if (state == "LeftTrigger")
		return GamePadAnalog::kLeftTrigger;
	if (state == "DPadDown")
		return GamePadAnalog::kDPadDown;
	if (state == "DPadLeft")
		return GamePadAnalog::kDPadLeft;
	if (state == "DPadUp")
		return GamePadAnalog::kDPadUp;
	if (state == "DPadRight")
		return GamePadAnalog::kDPadRight;

	return GamePadAnalog::kRightTrigger;
}

inline ButtonState ParseButtonState(const char* state)
{
	if (state == "Released")
		return ButtonState::kReleased;

	return ButtonState::kPressed;
}
*/