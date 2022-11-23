#include "../include/input.h"

#include "../include/memory_alloc.h"

Input *input;
InputActionEvent **input_action_events;
InputActionBinding **input_action_bindings;

InputAxisEvent **input_axis_events;
InputAxisBinding **input_axis_bindings;

struct controller_data keys_down;
struct controller_data keys_held;
struct controller_data keys_up;

static int controllers_connected;

bool input_is_controller_connected(InputController controller_id) {
	switch (controller_id) {
		case CONTROLLER_1:
			return controllers_connected & CONTROLLER_1_INSERTED;
		case CONTROLLER_2:
			return controllers_connected & CONTROLLER_2_INSERTED;
		case CONTROLLER_3:
			return controllers_connected & CONTROLLER_3_INSERTED;
		case CONTROLLER_4:
			return controllers_connected & CONTROLLER_4_INSERTED;
		default:
			return false;
	}
}

void input_init(MemZone *global_memory_pool, uint8_t actions_max_count,
				uint8_t actions_max_bindings_count) {
	controller_init();

	input = MEM_ALLOC(sizeof(Input), global_memory_pool);
	input->deadzone = 5.f;
	input->max_analog = 60.f;
	input->actions_max_events_count = actions_max_count;
	input->actions_max_bindings_count = actions_max_bindings_count;
	for (InputController i = CONTROLLER_1; i < CONTROLLER_MAX; ++i) {
		input->controller_instances[i] = NULL;
	}

	input_action_events = mem_zone_alloc(global_memory_pool,
										 sizeof(InputActionEvent *) * actions_max_count);
	input_action_bindings = MEM_ALLOC(sizeof(InputActionBinding *) * actions_max_bindings_count,
									  global_memory_pool);

	input_axis_events = MEM_ALLOC(sizeof(InputAxisEvent *) * actions_max_count, global_memory_pool);
	input_axis_bindings = MEM_ALLOC(sizeof(InputAxisBinding *) * actions_max_bindings_count,
									global_memory_pool);
}

void input_set_user_instance(InputController controller_id, void *instance) {
	input->controller_instances[controller_id] = instance;
}

void input_set_deadzone(float deadzone) {
	input->deadzone = deadzone;
}

void input_set_max_analog(float max_analog) {
	input->max_analog = max_analog;
}

void input_remove_all_bindings() {
	input->current_action_bindings = 0;
	input->current_axis_bindings = 0;
}

void input_update_controllers_connected() {
	static int counter = 60;
	counter++;
	if (counter >= 60) {
		counter = 0;
		controllers_connected = get_controllers_present();
	}
}

bool input_button_is_on_state(InputController controller_id, gamepad_button button,
							  gamepad_button_state state) {
	struct controller_data *data;
	if (state == gp_pressed) {
		data = &keys_down;
	} else if (state == gp_released) {
		data = &keys_up;
	} else {
		data = &keys_held;
	}

	switch (button) {
		case gp_A:
			return data->c[controller_id].A;
		case gp_B:
			return data->c[controller_id].B;
		case gp_Z:
			return data->c[controller_id].Z;
		case gp_start:
			return data->c[controller_id].start;
		case gp_up:
			return data->c[controller_id].up;
		case gp_down:
			return data->c[controller_id].down;
		case gp_left:
			return data->c[controller_id].left;
		case gp_right:
			return data->c[controller_id].right;
		case gp_L:
			return data->c[controller_id].L;
		case gp_R:
			return data->c[controller_id].R;
		case gp_C_up:
			return data->c[controller_id].C_up;
		case gp_C_down:
			return data->c[controller_id].C_down;
		case gp_C_left:
			return data->c[controller_id].C_left;
		case gp_C_right:
			return data->c[controller_id].C_right;
		default:
			return false;
	}
}

void input_update() {
	controller_scan();
	input_update_controllers_connected();

	keys_down = get_keys_down();
	keys_held = get_keys_held();
	keys_up = get_keys_up();

	for (uint8_t event_id = 0; event_id < input->current_action_events; ++event_id) {
		if (input_action_events[event_id]->is_paused)
			continue;

		for (InputController controller_id = CONTROLLER_1; controller_id < CONTROLLER_MAX;
			 ++controller_id) {
			if (!input_is_controller_connected(controller_id))
				continue;

			for (uint8_t bind_id = 0; bind_id < input->current_action_bindings; ++bind_id) {
				if (input_action_bindings[bind_id]->controller_id == controller_id &&
					input_action_events[event_id]->action_id ==
						input_action_bindings[bind_id]->action_id &&
					input_button_is_on_state(controller_id, input_action_bindings[bind_id]->button,
											 input_action_events[event_id]->state)) {
					input_action_events[event_id]->action_callback(
						input->controller_instances[controller_id]);
					break;
				}
			}
		}
	}

	for (uint8_t event_id = 0; event_id < input->current_axis_events; ++event_id) {
		if (input_axis_events[event_id]->is_paused)
			continue;

		for (InputController controller_id = CONTROLLER_1; controller_id < CONTROLLER_MAX;
			 ++controller_id) {
			if (!input_is_controller_connected(controller_id))
				continue;

			bool action_done = false;
			bool has_binding = false;

			for (uint8_t bind_id = 0; bind_id < input->current_axis_bindings; ++bind_id) {
				if (controller_id != input_axis_bindings[bind_id]->controller_id)
					continue;

				if (input_axis_bindings[bind_id]->action_id !=
					input_axis_events[event_id]->action_id)
					continue;

				has_binding = true;

				if (input_axis_bindings[bind_id]->button == gp_analog_horizontal ||
					input_axis_bindings[bind_id]->button == gp_analog_vertical) {
					int force = (input_axis_bindings[bind_id]->button == gp_analog_horizontal)
									? keys_held.c[controller_id].x
									: keys_held.c[controller_id].y;
					if (force > input->max_analog)
						force = input->max_analog;
					if (force < -input->max_analog)
						force = -input->max_analog;
					if (force < -input->deadzone || force > input->deadzone) {
						input_axis_events[event_id]->axis_callback(
							input->controller_instances[controller_id],
							force * input_axis_bindings[bind_id]->scale);
						action_done = true;
						break;
					}
				} else {
					bool held = input_button_is_on_state(
						controller_id, input_axis_bindings[bind_id]->button, gp_held);
					if (held) {
						input_axis_events[event_id]->axis_callback(
							input->controller_instances[controller_id],
							input->max_analog * input_axis_bindings[bind_id]->scale);
						action_done = true;
						break;
					}
				}
			}

			if (!action_done && has_binding) {
				input_axis_events[event_id]->axis_callback(
					input->controller_instances[controller_id], 0);
			}
		}
	}
}

void input_add_action_event(MemZone *global_memory_pool, uint8_t action_id,
							fnIInputActionCallback action_callback, gamepad_button_state state) {
	assertf(input->current_action_events < input->actions_max_events_count,
			"Trying to add more action_events than set on 'input_init'");

	uint8_t cur_action = input->current_action_events;
	if (input_action_events[cur_action] == NULL)
		input_action_events[cur_action] = MEM_ALLOC(sizeof(InputActionEvent), global_memory_pool);
	input_action_events[cur_action]->action_id = action_id;
	input_action_events[cur_action]->action_callback = action_callback;
	input_action_events[cur_action]->state = state;

	input->current_action_events++;
}

void input_add_axis_event(MemZone *global_memory_pool, uint8_t action_id,
						  fnIInputAxisCallback axis_callback) {
	assertf(input->current_axis_events < input->actions_max_events_count,
			"Trying to add more axis_events than set on 'input_init'");

	uint8_t cur_action = input->current_axis_events;
	if (input_axis_events[cur_action] == NULL)
		input_axis_events[cur_action] = MEM_ALLOC(sizeof(InputAxisEvent), global_memory_pool);
	input_axis_events[cur_action]->action_id = action_id;
	input_axis_events[cur_action]->axis_callback = axis_callback;

	input->current_axis_events++;
}

void input_add_action_binding(MemZone *global_memory_pool, gamepad_button button, uint8_t action_id,
							  InputController controller_id) {
	assertf(input->current_action_bindings < input->actions_max_bindings_count,
			"Trying to add more action_bindings than set on 'input_init'");

	uint8_t cur_action = input->current_action_bindings;
	if (input_action_bindings[cur_action] == NULL)
		input_action_bindings[cur_action] = MEM_ALLOC(sizeof(InputActionBinding),
													  global_memory_pool);
	input_action_bindings[cur_action]->button = button;
	input_action_bindings[cur_action]->action_id = action_id;
	input_action_bindings[cur_action]->controller_id = controller_id;

	input->current_action_bindings++;
}

void input_add_axis_binding(MemZone *global_memory_pool, gamepad_button button, float scale,
							uint8_t action_id, InputController controller_id) {
	assertf(input->current_axis_bindings < input->actions_max_bindings_count,
			"Trying to add more axis_bindings than set on 'input_init'");

	uint8_t cur_action = input->current_axis_bindings;
	if (input_axis_bindings[cur_action] == NULL)
		input_axis_bindings[cur_action] = MEM_ALLOC(sizeof(InputAxisBinding), global_memory_pool);
	input_axis_bindings[cur_action]->button = button;
	input_axis_bindings[cur_action]->scale = scale;
	input_axis_bindings[cur_action]->action_id = action_id;
	input_axis_bindings[cur_action]->controller_id = controller_id;

	input->current_axis_bindings++;
}

void input_axis_event_pause(uint8_t action_id) {
	for (uint8_t i = 0; i < input->current_axis_events; ++i) {
		if (input_axis_events[i]->action_id == action_id) {
			input_axis_events[i]->is_paused = true;
			return;
		}
	}
}

void input_axis_event_resume(uint8_t action_id) {
	for (uint8_t i = 0; i < input->current_axis_events; ++i) {
		if (input_axis_events[i]->action_id == action_id) {
			input_axis_events[i]->is_paused = false;
			return;
		}
	}
}

void input_action_event_pause(uint8_t action_id) {
	for (uint8_t i = 0; i < input->current_action_events; ++i) {
		if (input_action_events[i]->action_id == action_id) {
			input_action_events[i]->is_paused = true;
			return;
		}
	}
}

void input_action_event_resume(uint8_t action_id) {
	for (uint8_t i = 0; i < input->current_axis_events; ++i) {
		if (input_action_events[i]->action_id == action_id) {
			input_action_events[i]->is_paused = false;
			return;
		}
	}
}
