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
 * @brief Struct that holds an integer size.
 */
typedef struct {
	/// Width of the struct
	int width;
	/// Height of the struct
	int height;
} SizeInt;

/**
 * @brief Returns a new size where width and height are zero.
 */
inline Size new_size_zero() {
	Size size = {.width = 0.f, .height = 0.f};
	return size;
}

/**
 * @brief Returns a new size int where width and height are zero.
 */
inline SizeInt new_sizeint_zero() {
	SizeInt size = {.width = 0, .height = 0};
	return size;
}

/**
 * @brief Returns a new size where width and height are the same value.
 *
 * @param[in] width_and_height
 *            Value to set width and height with.
 */
inline Size new_size_same(float width_and_height) {
	Size size = {.width = width_and_height, .height = width_and_height};
	return size;
}

/**
 * @brief Returns a new size int where width and height are the same value.
 *
 * @param[in] width_and_height
 *            Value to set width and height with.
 */
inline SizeInt new_sizeint_same(int width_and_height) {
	SizeInt size = {.width = width_and_height, .height = width_and_height};
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
	Size size = {.width = width, .height = height};
	return size;
}

/**
 * @brief Returns a new size int where width and height are the values sent.
 *
 * @param[in] width
 *            Value to set width.
 * @param[in] height
 *            Value to set height.
 */
inline SizeInt new_sizeint(int width, int height) {
	SizeInt size = {.width = width, .height = height};
	return size;
}

#ifdef __cplusplus
}
#endif