#pragma once

#include <libdragon.h>
#include "mem_pool.h"
#include "rect.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct that holds a SpriteBatch.
 */
typedef struct {
	/// Where they will be rendered.
	Position *positions;
	/// Sprite used to render the batch.
	sprite_t *sprite;
	/// Amount of sprites to render.
	size_t qty;
	/// Size of the sprites.
	Size size;
	/// Offset to render the sprites.
	Position render_offset;
} SpriteBatch;

/**
 * @brief Allocates and initializes a SpriteBatch.
 *
 * @param memory_pool
 *        MemZone to use to allocate the SpriteBatch. If NULL will use 'malloc' and you have to free
 * memory using 'sprite_batch_destroy'.
 * @param sprite
 *        Sprite used to render the batch.
 * @param qty
 *        Amount of sprites to render.
 * @param size
 *        Size of the sprites.
 * @param render_offset
 *        Offset to render the sprites.
 *
 * @return The new SpriteBatch.
 */
SpriteBatch *sprite_batch_init(MemZone *memory_pool, sprite_t *sprite, size_t qty, Size size,
							   Position render_offset);

/**
 * @brief Draw the SpriteBatch on the screen.
 *
 * @param sprite_batch
 *        SpriteBatch to render.
 * @param offset
 *        Current frame to render for all sprites.
 * @param screen_rect
 *        Rect of the current screen. Used to check if the sprite is on the screen.
 */
void sprite_batch_draw(SpriteBatch *sprite_batch, int offset, Rect screen_rect);

/**
 * @brief Used to free memory allocated from SpriteBatch. Only use if no MemZone was used on
 * 'sprite_batch_init'.
 *
 * @param sprite_batch
 *        SpriteBatch to destroy.
 */
void sprite_batch_destroy(SpriteBatch *sprite_batch);

#ifdef __cplusplus
}
#endif