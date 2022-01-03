#pragma once

#include <stdint.h>
#include <libdragon.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Returns a new color using the specified parameters.
 *
 * @param r
 *        Red amount.
 * @param g
 *        Green amount.
 * @param b
 *        Blue amount.
 * @param a
 *        Alpha amount.
 *
 * @return New color.
 */
color_t new_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	color_t color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}

#ifdef __cplusplus
}
#endif