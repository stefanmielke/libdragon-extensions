#pragma once

#include <stdbool.h>
#include "mem_pool.h"

typedef enum { TWEEN_NONE, TWEEN_FLOAT } TweenType;

// callbacks per type
typedef void (*fnTWCallbackFloat)(void *target_object, float current_value);

typedef struct {
	void *target_object;
	void *tween_values;
	bool finished;
	TweenType type;
	MemZone *allocator;
} Tween;

Tween *tween_init(MemZone *memory_pool, void *target_object);
void tween_destroy(Tween *tween);
void tween_tick(Tween *tween);

void tween_start_to_float(Tween *tween, float start_value, float end_value, float duration_in_ms,
						  fnTWCallbackFloat tween_callback);
