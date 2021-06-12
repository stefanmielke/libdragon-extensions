#pragma once

#include <libdragon.h>
#include "mem_pool.h"
#include "rect.h"

typedef struct {
	char *map;
	Size map_size;
	Size tile_size;
	sprite_t *sprite;
} Tiled;

// Init a Tiled map
Tiled *tiled_init(MemZone *memory_pool, sprite_t *sprite, const char *map_path, Size map_size,
				  Size tile_size);

// Render a Tiled map (uses software rendering since it's faster for this)
void tiled_render(display_context_t disp, Tiled *tiled, Rect screen_rect);
