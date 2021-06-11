#pragma once

typedef struct {
	float width;
	float height;
} Size;

inline Size new_size_zero() {
	Size size;
	size.width = 0;
	size.height = 0;
	return size;
}

inline Size new_size_same(float width_and_height) {
	Size size;
	size.width = width_and_height;
	size.height = width_and_height;
	return size;
}

inline Size new_size(float width, float height) {
	Size size;
	size.width = width;
	size.height = height;
	return size;
}