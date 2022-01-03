#pragma once

#include <stdbool.h>
#include "position.h"
#include "size.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct that has a position and a size.
 */
typedef struct {
	/// Where the Rect is
	Position pos;
	/// Size of the Rect
	Size size;
} Rect;

/**
 * @brief Returns a new Rect with the position and size informed.
 *
 * @param[in] pos
 *            Position of the Rect
 * @param[in] size
 *            Size of the Rect
 */
inline Rect new_rect(Position pos, Size size) {
	Rect rect;
	rect.pos = pos;
	rect.size = size;
	return rect;
}

/**
 * @brief Check if rect 'a' intersects with rect 'b'.
 *
 * @return If 'a' is intersecting 'b'.
 */
inline bool is_intersecting(Rect a, Rect b) {
	return a.pos.x < b.pos.x + b.size.width && a.pos.x + a.size.width > b.pos.x &&
		   a.pos.y < b.pos.y + b.size.height && a.pos.y + a.size.height > b.pos.y;
}

/**
 * @brief Check if 'inner' is completely inside 'outer'.
 *
 * @param[in] inner
 *            Rectangle that should be inside.
 * @param[in] outer
 *            Rectangle that should contain 'inner'.
 * @return If 'inner' is completely inside 'outer'.
 */
inline bool contains(Rect inner, Rect outer) {
	return inner.pos.x >= outer.pos.x &&
		   inner.pos.x + inner.size.width <= outer.pos.x + outer.size.width &&
		   inner.pos.y >= outer.pos.y &&
		   inner.pos.y + inner.size.height <= outer.pos.y + outer.size.height;
}

#ifdef __cplusplus
}
#endif