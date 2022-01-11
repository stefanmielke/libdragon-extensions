#pragma once

#include "random.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct that has an int range.
 */
typedef struct {
	/// Start of the range
	int start;
	/// End of the range
	int end;
} RangeInt;

/**
 * @brief Generates a new RangeInt where start and end are the values sent.
 *
 * @param start
 *        Start of the range.
 * @param end
 *        End of the range.
 */
inline RangeInt new_range_int(float start, float end) {
	RangeInt range;
	range.start = start;
	range.end = end;
	return range;
}

/**
 * @brief Generates a new RangeInt where start and end are the same value.
 *
 * @param start_and_end
 *        Start and end of the range.
 */
inline RangeInt new_range_int_same(float start_and_end) {
	RangeInt range;
	range.start = start_and_end;
	range.end = start_and_end;
	return range;
}

/**
 * @brief Get a random value from the range.
 *
 * @param range
 *        Range to get start and end from.
 */
inline int range_get_from_int(RangeInt *range) {
	return RANDR(range->start, range->end);
}

#ifdef __cplusplus
}
#endif