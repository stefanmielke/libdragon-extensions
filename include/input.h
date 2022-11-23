#pragma once

#include <libdragon.h>

#include "input_enums.h"
#include "mem_pool.h"

typedef enum InputController {
	CONTROLLER_1 = 0,
	CONTROLLER_2,
	CONTROLLER_3,
	CONTROLLER_4,
	CONTROLLER_MAX,
} InputController;

/**
 * @brief Called during input_update when the action is performed.
 */
typedef void (*fnIInputActionCallback)(void *instance);

/**
 * @brief Called during input_update every frame.
 */
typedef void (*fnIInputAxisCallback)(void *instance, int scale);

typedef struct InputActionEvent {
	/// controller used for this action.
	InputController controller_id;
	/// id of the action.
	uint8_t action_id;
	/// callback function that will be triggered when this action happens
	fnIInputActionCallback action_callback;
	/// user object/state that will be passed to the callback.
	void *instance;
	/// type of the trigger (when the button is pressed or released)
	gamepad_button_state state;
	/// if this event will not generate callbacks
	bool is_paused;
} InputActionEvent;

typedef struct InputActionBinding {
	/// button bound to the action
	gamepad_button button;
	/// id of the action that will be triggered by the button
	uint8_t action_id;
} InputActionBinding;

typedef struct InputAxisEvent {
	/// controller used for this action.
	InputController controller_id;
	/// id of the action.
	uint8_t action_id;
	/// callback function that will be triggered every frame
	fnIInputAxisCallback axis_callback;
	/// user object/state that will be passed to the callback.
	void *instance;
	/// if this event will not generate callbacks
	bool is_paused;
} InputAxisEvent;

typedef struct InputAxisBinding {
	/// button bound to the action
	gamepad_button button;
	/// multiplier for the axis (normally from 0 to 1)
	float scale;
	/// id of the action that will be triggered by the button
	uint8_t action_id;
} InputAxisBinding;

typedef struct Input {
	/// minimum value required by the axis to start an AxisEvent. Default value is 5
	float deadzone;
	/// maximum value that will be passed to the AxisEvent callback function. Default is 60
	float max_analog;
	/// maximum amount of action events that can be bound
	uint8_t actions_max_events_count;
	/// maximum amount of action bindings that can be bound
	uint8_t actions_max_bindings_count;

	/// current count of action events
	uint8_t current_action_events;
	/// current count of action bindings
	uint8_t current_action_bindings;

	/// current count of axis events
	uint8_t current_axis_events;
	/// current count of axis bindings
	uint8_t current_axis_bindings;
} Input;

/// Input singleton
extern Input *input;
/// Action Events singleton
extern InputActionEvent **input_action_events;
/// Action Bindings singleton
extern InputActionBinding **input_action_bindings;

/// Axis Events singleton
extern InputAxisEvent **input_axis_events;
/// Axis Bindings singleton
extern InputAxisBinding **input_axis_bindings;

/// Current state of keys pressed (updated by input_update)
extern struct controller_data keys_down;
/// Current state of keys held down (updated by input_update)
extern struct controller_data keys_held;
/// Current state of keys released (updated by input_update)
extern struct controller_data keys_up;

/**
 * @brief Initializes the Input System. Should be called at the start of the game.
 *
 * @param[in] global_memory_pool
 *            The global memory pool. If NULL will use malloc.
 * @param[in] actions_max_count
 *            Max count for actions. This will determine the size of the arrays.
 * @return Input* The Input struct initialized.
 */
void input_init(MemZone *global_memory_pool, uint8_t actions_max_count,
				uint8_t actions_max_bindings);

/**
 * @brief Updates the input objects and call any callbacks that need to be triggered.
 */
void input_update();

/**
 * @brief Sets the axis deadzone. Default is 5.
 * The usual values from an N64 controller range from 0 to ~70, but varies a lot per controller.
 * The deadzone should be set to a safe value due to the analog wearing down after usage.
 *
 * @param deadzone The deadzone.
 */
void input_set_deadzone(float deadzone);
/**
 * @brief Sets the maximum value that will be sent to the Axis event callback functions.
 * The usual values from an N64 controller range from 0 to ~70, but varies a lot per controller.
 * The max analog should be set to a safe value that can be triggered by almost every controller.
 *
 * @param max_analog The maximum value that will be sent to the Axis event callback functions.
 */
void input_set_max_analog(float max_analog);

/**
 * @brief Remove all current bindings for Buttons and Analogs.
 * After calling this function you should call input_add_action_binding and input_add_axis_binding
 * again with the new values.
 */
void input_remove_all_bindings();

/**
 * @brief Return if the controller with the ID is connected.
 *
 * @param[in] controller_id Id of the controller.
 * @return true If the controller is connected.
 * @return false If the controller is not connected.
 */
bool input_is_controller_connected(InputController controller_id);

// /* TODO: add param here */ void input_serialize();
// void input_deserialize(/* TODO: add param here */);

/**
 * @brief Adds an action event bound to a callback that will be triggered when the bound action
 * binding happens.
 *
 * @param[in] global_memory_pool
 *            The global memory pool. If NULL will use malloc.
 * @param[in] controller_id
 *            The id of the controller port used.
 * @param[in] action_id
 *            Id of the action. This id is user controlled. Ideally an enum should be used.
 * @param[in] action_callback
 *            The function that will be called when the event happens.
 * @param[in] instance
 *            A user defined object/state that will be sent to the callback when the event happens.
 * Can be NULL.
 * @param[in] state
 *            Type of state that will trigger the event (gp_released or gp_pressed).
 */
void input_add_action_event(MemZone *global_memory_pool, InputController controller_id, uint8_t action_id,
							fnIInputActionCallback action_callback, void *instance,
							gamepad_button_state state);

/**
 * @brief Adds an action event bound to a callback that will be triggered every frame.
 *
 * @param global_memory_pool
 *            The global memory pool. If NULL will use malloc.
 * @param controller_id
 *            The id of the controller port used.
 * @param action_id
 *            Id of the action. This id is user controlled. Ideally an enum should be used.
 * @param axis_callback
 *            The function that will be called every frame with the updated value.
 * @param instance
 *            A user defined object/state that will be sent to the callback when the event happens.
 * Can be NULL.
 */
void input_add_axis_event(MemZone *global_memory_pool, InputController controller_id, uint8_t action_id,
						  fnIInputAxisCallback axis_callback, void *instance);

/**
 * @brief Binds a button to cause an action added by input_add_action_event.
 *
 * @param global_memory_pool
 *            The global memory pool. If NULL will use malloc.
 * @param button
 *            Button that will trigger the action.
 * @param action_id
 *            Id of the action. This id is user controlled. Ideally an enum should be used.
 */
void input_add_action_binding(MemZone *global_memory_pool, gamepad_button button,
							  uint8_t action_id);

/**
 * @brief Binds a button to cause an action added by input_add_action_event.
 *
 * @param global_memory_pool
 *            The global memory pool. If NULL will use malloc.
 * @param button
 *            Button that will trigger the action.
 * @param scale
 *            Multiplier that will be used when calling the axis_callback. This can be useful when
 * binding non-analog buttons. e.g.: gp_up can be 1, and gp_down can be -1.
 * @param action_id
 *            Id of the action. This id is user controlled. Ideally an enum should be used.
 */
void input_add_axis_binding(MemZone *global_memory_pool, gamepad_button button, float scale,
							uint8_t action_id);

/**
 * @brief Pause all axis callbacks for a given action. Call input_axis_event_resume to resume.
 *
 * @param action_id Id of the action that will be paused.
 */
void input_axis_event_pause(uint8_t action_id);

/**
 * @brief Resume all axis callbacks for a given action. Call input_axis_event_pause to pause again.
 *
 * @param action_id Id of the action that will be resumed.
 */
void input_axis_event_resume(uint8_t action_id);

/**
 * @brief Pause all action callbacks for a given action. Call input_action_event_resume to resume.
 *
 * @param action_id Id of the action that will be paused.
 */
void input_action_event_pause(uint8_t action_id);

/**
 * @brief Resume all action callbacks for a given action. Call input_action_event_pause to pause
 * again.
 *
 * @param action_id Id of the action that will be resumed.
 */
void input_action_event_resume(uint8_t action_id);
