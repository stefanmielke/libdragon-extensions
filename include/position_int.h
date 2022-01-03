#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct that has a position with int values.
 */
typedef struct {
	/// X coordinate
	int x;
	/// Y coordinate
	int y;
} PositionInt;

/**
 * @brief Returns a new position where X and Y are zero.
 */
inline PositionInt new_position_int_zero() {
	PositionInt pos;
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
inline PositionInt new_position_int_same(int x_and_y) {
	PositionInt pos;
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
inline PositionInt new_position_int(int x, int y) {
	PositionInt pos;
	pos.x = x;
	pos.y = y;
	return pos;
}

#ifdef __cplusplus
}
#endif