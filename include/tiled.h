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
// On my tests, you can load a map with around 2_500 (50x50) tiles before an error occurs
Tiled *tiled_init(MemZone *memory_pool, sprite_t *sprite, const char *map_path, Size map_size,
				  Size tile_size);

// Render a Tiled map (uses software rendering since it's faster for this)
// Use this for constant timing, and when you have lots of different textures
void tiled_render(display_context_t disp, Tiled *tiled, Rect screen_rect);

// Render a Tiled map (uses hardware rendering)
// Use this when there's not much texture swapping
void tiled_render_rdp(Tiled *tiled, Rect screen_rect);
