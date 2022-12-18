#pragma once

#include <libdragon.h>
#include "mem_pool.h"
#include "rect.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct with a sprite that can be animated.
 */
typedef struct {
	/// Sprite that this will use. Needs to have offsets to use.
	sprite_t *sprite;
	/// First offset for the animation
	size_t offset_start;
	/// Last offset for the animation
	size_t offset_end;
	/// Rate of frame change in ms
	float anim_speed;
	/// Size of the frame (to check when drawing)
	Size size;
	/// Offset of the frame (to use when rendering)
	Position render_offset;

	/// Current offset
	float _current_offset;
	/// Tick from last frame
	unsigned long _last_tick;
	/// Total sum of ticks
	unsigned long _ticks;
} AnimatedSprite;

/**
 * @brief Allocates and returns a new AnimatedSprite object.
 *
 * @param memory_pool
 *        MemZone to use when allocating memory.
 * @param sprite
 *        The sprite that will be used to render. Should have all the frames.
 * @param size
 *        Size of each frame.
 * @param render_offset
 *        Offset to be used when rendering.
 * @param offset_start
 *        Offset of the first frame.
 * @param offset_end
 *        Offset of the last frame.
 * @param anim_speed
 *        Speed of the animation in ms.
 *
 * @return The new AnimatedSprite.
 */
AnimatedSprite *animated_sprite_init(MemZone *memory_pool, sprite_t *sprite, Size size,
									 Position render_offset, size_t offset_start, size_t offset_end,
									 float anim_speed);

/**
 * @brief Updates the animation. Should be called on every frame.
 *
 * @param anim
 *        AnimatedSprite to tick.
 * @param anim_rate
 *        Rate that the animation should update this tick. Multiples 'anim_speed'.
 */
void animated_sprite_tick(AnimatedSprite *anim, float anim_rate);

/**
 * @brief Draw this AnimatedSprite as a textured rectangle.
 * 		  Uses hardware rendering and supports multiple formats.
 *
 * @param anim
 *        AnimatedSprite to draw.
 * @param pos
 *        Position of the AnimatedSprite.
 * @param screen_rect
 *        Rect of the current screen. Used to check if the AnimatedSprite is on the screen.
 */
void animated_sprite_draw(AnimatedSprite *anim, Position pos, Rect screen_rect);

#ifdef __cplusplus
}
#endif