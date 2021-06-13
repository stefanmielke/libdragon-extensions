#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "mem_pool.h"

/**
 * @brief Enum with the types of Tweens available.
 */
typedef enum { TWEEN_NONE, TWEEN_FLOAT } TweenType;

/**
 * @brief Easing function callback. See 'easing.h' for functions.
 *
 */
typedef float (*fnTWEasingFunction)(float value);

/**
 * @brief Callback for TWEEN_FLOAT.
 *
 * @param[in] target_object
 *            Object sent on 'tween_init'.
 * @param[in] current_value
 *            Current float value based on the easing function.
 */
typedef void (*fnTWCallbackFloat)(void *target_object, float current_value);

/**
 * @brief Struct for the Tween
 */
typedef struct {
	/// Object that should change.
	void *target_object;
	/// Pointer to the values of the tween function according to the type.
	void *tween_values;
	/// If the easing was finished or not.
	bool finished;
	/// Type of the tween. See tween_init.
	TweenType type;
	/// Memory pool used to allocate the structs. NULL if none should be used.
	MemZone *allocator;
	/// Easing function. See tween_init.
	fnTWEasingFunction easing_function;
} Tween;

/**
 * @brief Initializes the Tween.
 *
 * @param memory_pool
 *        Memory pool that should be used to allocate memory. Can be NULL if none should be used.
 * @param target_object
 *        Object that will change. Will be used on the calledback during tween_tick.
 * @param easing_function
 *        Function that should be used to ease the values. See easing.h.
 *
 * @return The new Tween.
 */
Tween *tween_init(MemZone *memory_pool, void *target_object, fnTWEasingFunction easing_function);

/**
 * @brief Destroy the memory allocated for the Tween.
 *
 * @param tween
 *        Tween to be destroyed.
 */
void tween_destroy(Tween *tween);

/**
 * @brief Updates the values for the Tween and calls back the function set with the new value.
 *
 * @param tween
 *        Tween to update.
 */
void tween_tick(Tween *tween);

/**
 * @brief Allocates and start the Tween to update a float value.
 *
 * @param tween
 *        Tween already initialized that should be used.
 * @param start_value
 *        Starting value for the easing.
 * @param end_value
 *        End value for the easing.
 * @param duration_in_ms
 *        Time in ms that it should take to go from start_value to end_value.
 * @param tween_callback
 *        Callback that will be called on 'tween_tick' with the update value. (eg.: void
 * tween_callback(void *target_object, float current_value))
 */
void tween_start_to_float(Tween *tween, float start_value, float end_value, uint64_t duration_in_ms,
						  fnTWCallbackFloat tween_callback);
