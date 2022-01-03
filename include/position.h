#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct that has a position.
 */
typedef struct {
	/// X coordinate
	float x;
	/// Y coordinate
	float y;
} Position;

/**
 * @brief Returns a new position where X and Y are zero.
 */
inline Position new_position_zero() {
	Position pos;
	pos.x = 0;
	pos.y = 0;
	return pos;
}

/**
 * @brief Returns a new position where X and Y are the same value.
 *
 * @param[in] x_and_y
 *            Value to set X and Y with.
 */
inline Position new_position_same(float x_and_y) {
	Position pos;
	pos.x = x_and_y;
	pos.y = x_and_y;
	return pos;
}

/**
 * @brief Returns a new position where X and Y are the values sent.
 *
 * @param[in] x
 *            Value to set X.
 * @param[in] y
 *            Value to set Y.
 */
inline Position new_position(float x, float y) {
	Position pos;
	pos.x = x;
	pos.y = y;
	return pos;
}

#ifdef __cplusplus
}
#endif