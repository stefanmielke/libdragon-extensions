#pragma once

#include "random.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct that has a float range.
 */
typedef struct {
	/// Start of the range
	float start;
	/// End of the range
	float end;
} RangeFloat;

/**
 * @brief Generates a new RangeFloat where start and end are the values sent.
 *
 * @param start
 *        Start of the range.
 * @param end
 *        End of the range.
 */
inline RangeFloat new_range_float(float start, float end) {
	RangeFloat range;
	range.start = start;
	range.end = end;
	return range;
}

/**
 * @brief Generates a new RangeFloat where start and end are the same value.
 *
 * @param start_and_end
 *        Start and end of the range.
 */
inline RangeFloat new_range_float_same(float start_and_end) {
	RangeFloat range;
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
inline float range_get_from_float(RangeFloat *range) {
	return RANDRF(range->start, range->end);
}

#ifdef __cplusplus
}
#endif