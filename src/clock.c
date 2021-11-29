#include "../include/clock.h"
#include "../include/memory_alloc.h"

#include <libdragon.h>

Clock *new_clock(MemZone *memory_pool, int ms_per_tick, void (*callback)()) {
	Clock *clock = MEM_ALLOC(sizeof(Clock), memory_pool);
	clock->ticks_per_callback = ms_per_tick;
	clock->callback = callback;

	clock->last_ms = get_ticks_ms();
	clock->last_tick = 0;
	clock->current_time = 0;
	clock->is_paused = false;

	return clock;
}

void clock_destroy(Clock *clock) {
	free(clock);
}

void clock_tick(Clock *clock) {
	if (clock->is_paused)
		return;

	uint64_t current_ms = get_ticks_ms();
	uint64_t tick_since_last = current_ms - clock->last_ms;
	clock->current_time += tick_since_last;
	clock->last_ms = current_ms;

	if (clock->current_time >= clock->last_tick + clock->ticks_per_callback) {
		clock->callback();

		clock->last_tick = clock->current_time;
	}
}

void clock_pause(Clock *clock) {
	clock->is_paused = true;
}

void clock_resume(Clock *clock) {
	clock->is_paused = false;

	clock->last_ms = get_ticks_ms();
}