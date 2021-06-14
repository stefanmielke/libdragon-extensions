#pragma once

#include <stdint.h>
#include <libdragon.h>

color_t new_color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
	color_t color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}
