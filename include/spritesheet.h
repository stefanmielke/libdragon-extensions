#pragma once

#include <libdragon.h>
#include "mem_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Loads and allocate a sprite_t on the MemZone.
 *
 * @param memory_pool
 *        MemZone to allocate the sprite_t. If NULL will use 'malloc', remember to call 'free' if
 * you use that.
 * @param sprite_path
 *        Path to the sprite (eg.: "/sprites/my_sprite.sprite").
 *
 * @return The new sprite.
 */
inline sprite_t *spritesheet_load(MemZone *memory_pool, const char *sprite_path) {
	int fp = dfs_open(sprite_path);

	sprite_t *sprite = NULL;
	if (memory_pool)
		sprite = (sprite_t *)mem_zone_alloc(memory_pool, dfs_size(fp));
	else
		sprite = (sprite_t *)malloc(dfs_size(fp));

	dfs_read(sprite, 1, dfs_size(fp), fp);
	dfs_close(fp);

	return sprite;
}

#ifdef __cplusplus
}
#endif