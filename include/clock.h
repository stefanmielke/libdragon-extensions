#pragma once

#include <libdragon.h>

#include "mem_pool.h"

typedef void (*fnClockCallback)(void);

typedef struct Clock {
	int ticks_per_callback;
	fnClockCallback callback;

	uint64_t last_tick;
	uint64_t last_ms;
	uint64_t current_time;

	bool is_paused;
} Clock;

Clock *new_clock(MemZone *memory_pool, int ms_per_tick, fnClockCallback callback);
void clock_tick(Clock *clock);

void clock_pause(Clock *clock);
void clock_resume(Clock *clock);

void clock_destroy(Clock *clock);
