#include "../include/tween.h"

#include <libdragon.h>
#include "../include/easing.h"
#include "../include/memory_alloc.h"

void tween_float_tick(Tween *tween, float tick_diff);
void tween_float_swap(Tween *tween);

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

	switch (tween->type) {
		case TWEEN_FLOAT:
			tween_float_tick(tween, tick_diff);
			break;
		default:
			break;
	}

	if (tween->current_time >= tween->duration_in_ms) {
		if ((!tween->always_repeat && !tween->auto_reverse) ||
			(tween->auto_reverse && tween->is_reversing && !tween->always_repeat)) {
			if (tween->ending_callback)
				tween->ending_callback(tween->target_object);

			tween->finished = true;
			return;
		}

		if (tween->auto_reverse && (!tween->is_reversing || tween->always_repeat)) {
			tween->current_time = 0;
			tween->is_reversing = !tween->is_reversing;
			switch (tween->type) {
				case TWEEN_FLOAT:
					tween_float_swap(tween);
					break;
				default:
					break;
			}
			return;
		}

		if (tween->always_repeat) {
			tween->current_time = 0;
			return;
		}
	}
}

void tween_start(Tween *tween, void *target_object, fnTWEasingFunction easing_function,
				 uint64_t duration_in_ms, fnTWCallbackEnding tween_ending, bool auto_reverse,
				 bool always_repeat) {
	tween->type = TWEEN_FLOAT;
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

// Tween Float

typedef struct {
	float start_value;
	float end_value;
	float value_diff;
	fnTWCallbackFloat tween_callback;
} TweenValuesFloat;

void tween_set_to_float(Tween *tween, float start_value, float end_value,
						fnTWCallbackFloat tween_callback) {
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