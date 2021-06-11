#pragma once

typedef struct {
	float x;
	float y;
} Position;

inline Position new_position_zero() {
	Position pos;
	pos.x = 0;
	pos.y = 0;
	return pos;
}

inline Position new_position_same(float x_and_y) {
	Position pos;
	pos.x = x_and_y;
	pos.y = x_and_y;
	return pos;
}

inline Position new_position(float x, float y) {
	Position pos;
	pos.x = x;
	pos.y = y;
	return pos;
}
