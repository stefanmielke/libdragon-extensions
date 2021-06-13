#include "../include/tween.h"

#include <libdragon.h>
#include "../include/easing.h"
#include "../include/memory_alloc.h"

void tween_float_tick(Tween *tween);

Tween *tween_init(MemZone *memory_pool, void *target_object, fnTWEasingFunction easing_function) {
	Tween *tween = MEM_ALLOC(sizeof(Tween), memory_pool);
	tween->allocator = memory_pool;
	tween->target_object = target_object;
	tween->type = TWEEN_NONE;
	tween->finished = false;
	tween->easing_function = easing_function;

	return tween;
}

void tween_destroy(Tween *tween) {
	if (!tween->allocator) {
		free(tween->tween_values);
		free(tween);
	}
}

void tween_tick(Tween *tween) {
	if (tween->finished)
		return;

	switch (tween->type) {
		case TWEEN_FLOAT:
			tween_float_tick(tween);
			break;
		default:
			break;
	}
}

// Tween Float

typedef struct {
	float start_value;
	float end_value;
	float value_per_tick;
	uint64_t current_time;
	uint64_t duration_in_ticks;
	uint64_t start_tick;
	uint64_t last_tick;
	fnTWCallbackFloat tween_callback;
} TweenValuesFloat;

void tween_start_to_float(Tween *tween, float start_value, float end_value, uint64_t duration_in_ms,
						  fnTWCallbackFloat tween_callback) {
	tween->type = TWEEN_FLOAT;

	TweenValuesFloat *values = MEM_ALLOC(sizeof(TweenValuesFloat), tween->allocator);
	values->start_value = start_value;
	values->end_value = end_value;
	values->current_time = 0;
	values->duration_in_ticks = duration_in_ms;
	values->tween_callback = tween_callback;
	values->value_per_tick = (end_value - start_value) /*/ TICKS_FROM_MS(duration_in_ms)*/;
	values->start_tick = get_ticks_ms();
	values->last_tick = values->start_tick;

	tween->tween_values = values;
}

void tween_float_tick(Tween *tween) {
	if (!tween->tween_values)
		return;

	TweenValuesFloat *values = tween->tween_values;

	uint64_t current_tick = get_ticks_ms();
	uint64_t tick_since_last = current_tick - values->last_tick;
	values->current_time += tick_since_last;
	values->last_tick = current_tick;

	if (values->current_time >= values->duration_in_ticks) {
		tween->finished = true;
	}

	float tick_diff = values->current_time / (float)values->duration_in_ticks;

	tick_diff = tween->easing_function(tick_diff);

	float next_value = (tick_diff * values->value_per_tick) + values->start_value;
	values->tween_callback(tween->target_object, next_value);
}
