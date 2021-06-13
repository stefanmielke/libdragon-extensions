#pragma once

#include <libdragon.h>
#include "mem_pool.h"

/**
 * @brief Loads and allocate a sprite_t on the MemZone.
 *
 * @param memory_pool
 *        MemZone to allocate the sprite_t.
 * @param sprite_path
 *        Path to the sprite (eg.: "/sprites/my_sprite.sprite").
 *
 * @return The new sprite.
 */
sprite_t *load_spritesheet(MemZone *memory_pool, const char *sprite_path) {
	int fp = dfs_open(sprite_path);

	sprite_t *sprite = (sprite_t *)mem_zone_alloc(memory_pool, dfs_size(fp));

	dfs_read(sprite, 1, dfs_size(fp), fp);
	dfs_close(fp);

	return sprite;
}
