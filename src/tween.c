#include "../include/tween.h"

#include <libdragon.h>
#include "../include/memory_alloc.h"

void tween_float_tick(Tween *tween);

Tween *tween_init(MemZone *memory_pool, void *target_object) {
	Tween *tween = MEM_ALLOC(sizeof(Tween), memory_pool);
	tween->allocator = memory_pool;
	tween->target_object = target_object;
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
	float duration_in_ms;
	bool reversed;
	uint32_t start_tick;
	fnTWCallbackFloat tween_callback;
} TweenValuesFloat;

void tween_start_to_float(Tween *tween, float start_value, float end_value, float duration_in_ms,
						  fnTWCallbackFloat tween_callback) {
	tween->type = TWEEN_FLOAT;

	TweenValuesFloat *values = MEM_ALLOC(sizeof(TweenValuesFloat), tween->allocator);
	values->start_value = start_value;
	values->end_value = end_value;
	values->duration_in_ms = duration_in_ms;
	values->tween_callback = tween_callback;
	values->value_per_tick = (end_value - start_value) / TICKS_FROM_MS(duration_in_ms);
	values->start_tick = get_ticks();
	values->reversed = values->value_per_tick < 0;

	tween->tween_values = values;
}

void tween_float_tick(Tween *tween) {
	if (!tween->tween_values)
		return;

	TweenValuesFloat *values = tween->tween_values;

	uint32_t tick_diff = get_ticks() - values->start_tick;

	float next_value = tick_diff * values->value_per_tick;
	if (!values->reversed && next_value >= values->end_value) {
		next_value = values->end_value;
		tween->finished = true;
	} else if (values->reversed && next_value <= values->end_value) {
		next_value = values->end_value;
		tween->finished = true;
	}

	values->tween_callback(tween->target_object, next_value);
}
