#include "../include/tween.h"

#include <libdragon.h>
#include "../include/easing.h"
#include "../include/memory_alloc.h"

void tween_float_tick(Tween *tween, float tick_diff);
void tween_float_swap(Tween *tween);

void tween_size_tick(Tween *tween, float tick_diff);
void tween_size_swap(Tween *tween);

void tween_position_tick(Tween *tween, float tick_diff);
void tween_position_swap(Tween *tween);

void tween_color_tick(Tween *tween, float tick_diff);
void tween_color_swap(Tween *tween);

Tween *tween_init(MemZone *memory_pool) {
	Tween *tween = MEM_ALLOC(sizeof(Tween), memory_pool);
	tween->allocator = memory_pool;
	tween->type = TWEEN_NONE;
	tween->finished = false;

	return tween;
}

void tween_destroy(Tween *tween) {
	if (!tween->allocator) {
		free(tween->tween_values);
		free(tween);
	}
}

void tween_tick(Tween *tween) {
	if (tween->finished || !tween->tween_values)
		return;

	uint64_t current_ms = get_ticks_ms();
	uint64_t tick_since_last = current_ms - tween->last_ms;
	tween->current_time += tick_since_last;
	tween->last_ms = current_ms;

	float tick_diff = tween->current_time / (float)tween->duration_in_ms;

	tick_diff = tween->easing_function(tick_diff);

	bool should_end = tween->current_time >= tween->duration_in_ms;
	switch (tween->type) {
		case TWEEN_FLOAT:
			tween_float_tick(tween, tick_diff);
			break;
		case TWEEN_SIZE:
			tween_size_tick(tween, tick_diff);
			break;
		case TWEEN_POSITION:
			tween_position_tick(tween, tick_diff);
			break;
		case TWEEN_COLOR:
			tween_color_tick(tween, tick_diff);
			break;
		default:
			break;
	}

	if (should_end) {
		if ((!tween->always_repeat && !tween->auto_reverse) ||
			(tween->auto_reverse && tween->is_reversing && !tween->always_repeat)) {
			if (tween->ending_callback)
				tween->ending_callback(tween->target_object);

			tween->finished = true;
			return;
		}

		if (tween->auto_reverse && (!tween->is_reversing || tween->always_repeat)) {
			tween->current_time = 0;
			if (tween->is_reversing && tween->ending_callback) {
				tween->ending_callback(tween->target_object);
			}

			tween->is_reversing = !tween->is_reversing;
			switch (tween->type) {
				case TWEEN_FLOAT:
					tween_float_swap(tween);
					break;
				case TWEEN_SIZE:
					tween_size_swap(tween);
					break;
				case TWEEN_POSITION:
					tween_position_swap(tween);
					break;
				case TWEEN_COLOR:
					tween_color_swap(tween);
					break;
				default:
					break;
			}
			return;
		}

		if (tween->always_repeat) {
			tween->current_time = 0;
			if (tween->ending_callback)
				tween->ending_callback(tween->target_object);

			return;
		}
	}
}

void tween_start(Tween *tween, void *target_object, fnTWEasingFunction easing_function,
				 uint64_t duration_in_ms, fnTWCallbackEnding tween_ending, bool auto_reverse,
				 bool always_repeat) {
	tween->type = TWEEN_NONE;
	tween->target_object = target_object;
	tween->easing_function = easing_function;
	tween->finished = false;
	tween->current_time = 0;
	tween->duration_in_ms = duration_in_ms;
	tween->last_ms = get_ticks_ms();
	tween->ending_callback = tween_ending;
	tween->auto_reverse = auto_reverse;
	tween->is_reversing = false;
	tween->always_repeat = always_repeat;
}

void tween_change_duration(Tween *tween, uint64_t duration_in_ms) {
	tween->duration_in_ms = duration_in_ms;
}

// Tween Float

typedef struct {
	float start_value;
	float end_value;
	float value_diff;
	fnTWCallbackFloat tween_callback;
} TweenValuesFloat;

void tween_set_to_float(Tween *tween, float start_value, float end_value,
						fnTWCallbackFloat tween_callback) {
	tween->type = TWEEN_FLOAT;

	TweenValuesFloat *values = MEM_ALLOC(sizeof(TweenValuesFloat), tween->allocator);
	values->start_value = start_value;
	values->end_value = end_value;
	values->value_diff = end_value - start_value;
	values->tween_callback = tween_callback;

	tween->tween_values = values;
}

void tween_float_tick(Tween *tween, float tick_diff) {
	TweenValuesFloat *values = tween->tween_values;
	float next_value = (tick_diff * values->value_diff) + values->start_value;
	values->tween_callback(tween->target_object, next_value);
}

void tween_float_swap(Tween *tween) {
	TweenValuesFloat *values = tween->tween_values;
	float start_value = values->start_value;
	values->start_value = values->end_value;
	values->end_value = start_value;
	values->value_diff = values->end_value - values->start_value;
}

// Tween Size

typedef struct {
	Size start_value;
	Size end_value;
	Size value_diff;
	fnTWCallbackSize tween_callback;
} TweenValuesSize;

void tween_set_to_size(Tween *tween, Size start_value, Size end_value,
					   fnTWCallbackSize tween_callback) {
	tween->type = TWEEN_SIZE;

	TweenValuesSize *values = MEM_ALLOC(sizeof(TweenValuesSize), tween->allocator);
	values->start_value = start_value;
	values->end_value = end_value;
	values->value_diff.width = values->end_value.width - values->start_value.width;
	values->value_diff.height = values->end_value.height - values->start_value.height;
	values->tween_callback = tween_callback;

	tween->tween_values = values;
}

void tween_size_tick(Tween *tween, float tick_diff) {
	TweenValuesSize *values = tween->tween_values;
	float next_value_width = (tick_diff * values->value_diff.width) + values->start_value.width;
	float next_value_height = (tick_diff * values->value_diff.height) + values->start_value.height;
	values->tween_callback(tween->target_object, new_size(next_value_width, next_value_height));
}

void tween_size_swap(Tween *tween) {
	TweenValuesSize *values = tween->tween_values;
	Size start_value = values->start_value;
	values->start_value = values->end_value;
	values->end_value = start_value;
	values->value_diff.width = values->end_value.width - values->start_value.width;
	values->value_diff.height = values->end_value.height - values->start_value.height;
}

// Tween Position

typedef struct {
	Position start_value;
	Position end_value;
	Position value_diff;
	fnTWCallbackPosition tween_callback;
} TweenValuesPosition;

void tween_set_to_position(Tween *tween, Position start_value, Position end_value,
						   fnTWCallbackPosition tween_callback) {
	tween->type = TWEEN_POSITION;

	TweenValuesPosition *values = MEM_ALLOC(sizeof(TweenValuesPosition), tween->allocator);
	values->start_value = start_value;
	values->end_value = end_value;
	values->value_diff.x = values->end_value.x - values->start_value.x;
	values->value_diff.y = values->end_value.y - values->start_value.y;
	values->tween_callback = tween_callback;

	tween->tween_values = values;
}

void tween_position_tick(Tween *tween, float tick_diff) {
	TweenValuesPosition *values = tween->tween_values;
	float next_value_x = (tick_diff * values->value_diff.x) + values->start_value.x;
	float next_value_y = (tick_diff * values->value_diff.y) + values->start_value.y;
	values->tween_callback(tween->target_object, new_position(next_value_x, next_value_y));
}

void tween_position_swap(Tween *tween) {
	TweenValuesPosition *values = tween->tween_values;
	Position start_value = values->start_value;
	values->start_value = values->end_value;
	values->end_value = start_value;
	values->value_diff.x = values->end_value.x - values->start_value.x;
	values->value_diff.y = values->end_value.y - values->start_value.y;
}

// Tween Color

// diff in color can be negative
typedef struct {
	int16_t r;
	int16_t g;
	int16_t b;
	int16_t a;
} SignedColor;

typedef struct {
	color_t start_value;
	color_t end_value;
	SignedColor value_diff;
	fnTWCallbackColor tween_callback;
} TweenValuesColor;

void tween_set_to_color(Tween *tween, color_t start_value, color_t end_value,
						fnTWCallbackColor tween_callback) {
	tween->type = TWEEN_COLOR;

	TweenValuesColor *values = MEM_ALLOC(sizeof(TweenValuesColor), tween->allocator);
	values->start_value = start_value;
	values->end_value = end_value;
	values->value_diff.r = values->end_value.r - values->start_value.r;
	values->value_diff.g = values->end_value.g - values->start_value.g;
	values->value_diff.b = values->end_value.b - values->start_value.b;
	values->value_diff.a = values->end_value.a - values->start_value.a;
	values->tween_callback = tween_callback;

	tween->tween_values = values;
}

void tween_color_tick(Tween *tween, float tick_diff) {
	TweenValuesColor *values = tween->tween_values;
	uint8_t r = (tick_diff * values->value_diff.r) + values->start_value.r;
	uint8_t g = (tick_diff * values->value_diff.g) + values->start_value.g;
	uint8_t b = (tick_diff * values->value_diff.b) + values->start_value.b;
	uint8_t a = (tick_diff * values->value_diff.a) + values->start_value.a;
	values->tween_callback(tween->target_object, graphics_make_color(r, g, b, a));
}

void tween_color_swap(Tween *tween) {
	TweenValuesColor *values = tween->tween_values;
	color_t start_value = values->start_value;
	values->start_value = values->end_value;
	values->end_value = start_value;
	values->value_diff.r = values->end_value.r - values->start_value.r;
	values->value_diff.g = values->end_value.g - values->start_value.g;
	values->value_diff.b = values->end_value.b - values->start_value.b;
	values->value_diff.a = values->end_value.a - values->start_value.a;
}
