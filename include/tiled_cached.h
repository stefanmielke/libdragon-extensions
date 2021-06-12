#pragma once

#include <libdragon.h>
#include "mem_pool.h"
#include "rect.h"
#include "position_int.h"

typedef struct {
	size_t count;
	PositionInt *position;
} TiledCachedTile;

typedef struct {
	TiledCachedTile tiles[255];
	Size map_size;
	Size tile_size;
	sprite_t *sprite;
} TiledCached;

// Init a Tiled map
// On my tests, you can load a map with around 2_500 (50x50) tiles before an error occurs
TiledCached *tiled_cached_init(MemZone *memory_pool, sprite_t *sprite, const char *map_path,
							   Size map_size, Size tile_size);

// Render a Tiled map
void tiled_cached_render(TiledCached *tiled, Rect screen_rect);
