#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <libdragon.h>
#include "mem_pool.h"
#include "position.h"
#include "size.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Enum with the types of Tweens available.
 */
typedef enum { TWEEN_NONE, TWEEN_FLOAT, TWEEN_SIZE, TWEEN_POSITION, TWEEN_COLOR } TweenType;

/**
 * @brief Easing function callback. See 'easing.h' for functions.
 */
typedef float (*fnTWEasingFunction)(float value);

/**
 * @brief Callback for when the tween ends.
 *
 * If 'auto_reverse' is set, it is called when it finishes reversing.
 * If 'always_repeat' is set, it will never be called.
 */
typedef void (*fnTWCallbackEnding)(void *target_object);

/**
 * @brief Callback for TWEEN_FLOAT.
 *
 * @param[in] target_object
 *            Object sent on 'tween_init'.
 * @param[in] current_value
 *            Current value based on the easing function.
 */
typedef void (*fnTWCallbackFloat)(void *target_object, float current_value);

/**
 * @brief Callback for TWEEN_SIZE.
 *
 * @param[in] target_object
 *            Object sent on 'tween_init'.
 * @param[in] current_value
 *            Current value based on the easing function.
 */
typedef void (*fnTWCallbackSize)(void *target_object, Size current_value);

/**
 * @brief Callback for TWEEN_POSITION.
 *
 * @param[in] target_object
 *            Object sent on 'tween_init'.
 * @param[in] current_value
 *            Current value based on the easing function.
 */
typedef void (*fnTWCallbackPosition)(void *target_object, Position current_value);

/**
 * @brief Callback for TWEEN_COLOR.
 *
 * @param[in] target_object
 *            Object sent on 'tween_init'.
 * @param[in] current_value
 *            Current value based on the easing function. It's the result of 'graphics_make_color'.
 */
typedef void (*fnTWCallbackColor)(void *target_object, uint32_t current_value);

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
	/// If will start reversing at the end.
	bool auto_reverse;
	/// If it is reversing.
	bool is_reversing;
	/// Repeat when is completed.
	bool always_repeat;
	/// Type of the tween. See tween_init.
	TweenType type;
	/// Memory pool used to allocate the structs. NULL if none should be used.
	MemZone *allocator;
	/// Easing function. See tween_init.
	fnTWEasingFunction easing_function;
	/// Function called when the tween ends. Can be NULL.
	fnTWCallbackEnding ending_callback;
	/// Current time in ms since the start of the tween.
	uint64_t current_time;
	/// Total duration of the Tween in ms.
	uint64_t duration_in_ms;
	/// Last calculated ms.
	uint64_t last_ms;
} Tween;

/**
 * @brief Initializes the Tween.
 *
 * @param memory_pool
 *        Memory pool that should be used to allocate memory. Can be NULL if none should be used.
 *
 * @return The new Tween.
 */
Tween *tween_init(MemZone *memory_pool);

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
 * @brief Allocates and start the Tween. You should call 'tween_set_to_*' after this call to define
 * the Tween values.
 *
 * @param tween
 *        Tween already initialized that should be used.
 * @param target_object
 *        Object that will change. Will be used on the calledback during tween_tick.
 * @param easing_function
 *        Function that should be used to ease the values. See easing.h.
 * @param duration_in_ms
 *        Time in ms that it should take to go from start_value to end_value.
 * @param tween_ending
 *        Callback that will be called when the Tween ends (if auto_reverse is true, is only called
 * when the reverse ends). Can be NULL.
 * @param auto_reverse
 *        If it should go back after finishing. If set, will only call 'tween_ending' after it
 * finishes reversing.
 * @param always_repeat
 *        If it should loop infinitely.
 */
void tween_start(Tween *tween, void *target_object, fnTWEasingFunction easing_function,
				 uint64_t duration_in_ms, fnTWCallbackEnding tween_ending, bool auto_reverse,
				 bool always_repeat);

/**
 * @brief Changes duration of a Tween. Can cause weird effects if used outside of a
 * 'fnTWCallbackEnding'.
 *
 * @param tween
 *        Tween to update.
 * @param duration_in_ms
 *        New time in ms that it should take to go from start_value to end_value.
 */
void tween_change_duration(Tween *tween, uint64_t duration_in_ms);

/**
 * @brief Allocates and start the Tween to update a float value.
 *
 * @param tween
 *        Tween already initialized that should be used.
 * @param start_value
 *        Starting value for the easing.
 * @param end_value
 *        End value for the easing.
 * @param tween_callback
 *        Callback that will be called on 'tween_tick' with the update value. (eg.: void
 * tween_callback(void *target_object, float current_value))
 */
void tween_set_to_float(Tween *tween, float start_value, float end_value,
						fnTWCallbackFloat tween_callback);

/**
 * @brief Allocates and start the Tween to update a Size value.
 *
 * @param tween
 *        Tween already initialized that should be used.
 * @param start_value
 *        Starting value for the easing.
 * @param end_value
 *        End value for the easing.
 * @param tween_callback
 *        Callback that will be called on 'tween_tick' with the update value. (eg.: void
 * tween_callback(void *target_object, Size current_value))
 */
void tween_set_to_size(Tween *tween, Size start_value, Size end_value,
					   fnTWCallbackSize tween_callback);

/**
 * @brief Allocates and start the Tween to update a Position value.
 *
 * @param tween
 *        Tween already initialized that should be used.
 * @param start_value
 *        Starting value for the easing.
 * @param end_value
 *        End value for the easing.
 * @param tween_callback
 *        Callback that will be called on 'tween_tick' with the update value. (eg.: void
 * tween_callback(void *target_object, Position current_value))
 */
void tween_set_to_position(Tween *tween, Position start_value, Position end_value,
						   fnTWCallbackPosition tween_callback);

/**
 * @brief Allocates and start the Tween to update a Color value.
 *
 * @param tween
 *        Tween already initialized that should be used.
 * @param start_value
 *        Starting value for the easing.
 * @param end_value
 *        End value for the easing.
 * @param tween_callback
 *        Callback that will be called on 'tween_tick' with the update value. (eg.: void
 * tween_callback(void *target_object, color_t current_value))
 */
void tween_set_to_color(Tween *tween, color_t start_value, color_t end_value,
						fnTWCallbackColor tween_callback);

#ifdef __cplusplus
}
#endif