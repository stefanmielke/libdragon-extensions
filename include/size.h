#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct that holds size.
 */
typedef struct {
	/// Width of the struct
	float width;
	/// Height of the struct
	float height;
} Size;

/**
 * @brief Returns a new size where width and height are zero.
 */
inline Size new_size_zero() {
	Size size;
	size.width = 0;
	size.height = 0;
	return size;
}

/**
 * @brief Returns a new size where width and height are the same value.
 *
 * @param[in] width_and_height
 *            Value to set width and height with.
 */
inline Size new_size_same(float width_and_height) {
	Size size;
	size.width = width_and_height;
	size.height = width_and_height;
	return size;
}

/**
 * @brief Returns a new size where width and height are the values sent.
 *
 * @param[in] width
 *            Value to set width.
 * @param[in] height
 *            Value to set height.
 */
inline Size new_size(float width, float height) {
	Size size;
	size.width = width;
	size.height = height;
	return size;
}

#ifdef __cplusplus
}
#endif