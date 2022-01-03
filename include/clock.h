#pragma once

#include <libdragon.h>

#include "mem_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Function used as a base for the callback. Example: 'void my_callback();'
 */
typedef void (*fnClockCallback)(void);

/**
 * @brief Struct for the clock.
 */
typedef struct Clock {
	/// Amount of ticks that have to pass for the callback to be called.
	int ticks_per_callback;
	/// Callback function to be called.
	fnClockCallback callback;

	/// Time (in ticks) on the previous tick
	uint64_t last_tick;
	/// Time (in ms) on the previous tick
	uint64_t last_ms;
	/// Current time elapsed since the beginning (in ticks)
	uint64_t current_time;

	/// If the clock is paused
	bool is_paused;
} Clock;

/**
 * @brief Allocates and return a new Clock object.
 *
 * @param memory_pool
 *        MemZone to use when allocating memory. If NULL 'malloc' will be used.
 * @param ms_per_tick
 *        Amount of milliseconds that have to pass before calling the callback function.
 * @param callback
 *        Function that will be called after the time is elapsed.
 *
 * @return The new Clock.
 */
Clock *new_clock(MemZone *memory_pool, int ms_per_tick, fnClockCallback callback);

/**
 * @brief Ticks the clock. Should be called evey frame.
 *
 * @param clock
 *        Reference to the Clock object that will tick.
 */
void clock_tick(Clock *clock);

/**
 * @brief Pause the clock. When paused, the callback function will not be called.
 *
 * @param clock
 *        Reference to the Clock object that will be paused.
 */
void clock_pause(Clock *clock);
/**
 * @brief Resumes the clock after paused. Will not cause callbacks to be called.
 *
 * @param clock
 *        Reference to the Clock object that will be resumed.
 */
void clock_resume(Clock *clock);

/**
 * @brief Destroy a clock that was created. Should only be called when not using a MemZone.
 *
 * @param clock
 *        Reference to the Clock object that will be destroyed.
 */
void clock_destroy(Clock *clock);

#ifdef __cplusplus
}
#endif