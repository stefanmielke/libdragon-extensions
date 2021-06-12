#pragma once

#include <libdragon.h>
#include "mem_pool.h"

sprite_t *load_spritesheet(MemZone *memory_pool, const char *sprite_path) {
	int fp = dfs_open(sprite_path);

	sprite_t *sprite = (sprite_t *)mem_zone_alloc(memory_pool, dfs_size(fp));

	dfs_read(sprite, 1, dfs_size(fp), fp);
	dfs_close(fp);

	return sprite;
}
