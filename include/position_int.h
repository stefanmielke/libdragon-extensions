#pragma once

typedef struct {
	int x;
	int y;
} PositionInt;

inline PositionInt new_position_int_zero() {
	PositionInt pos;
	pos.x = 0;
	pos.y = 0;
	return pos;
}

inline PositionInt new_position_int_same(int x_and_y) {
	PositionInt pos;
	pos.x = x_and_y;
	pos.y = x_and_y;
	return pos;
}

inline PositionInt new_position_int(int x, int y) {
	PositionInt pos;
	pos.x = x;
	pos.y = y;
	return pos;
}
