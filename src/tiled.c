#include "../include/tiled.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_BUFFER_SIZE 100

// Init a Tiled map
Tiled *tiled_init(MemZone *memory_pool, sprite_t *sprite, const char *map_path, Size map_size,
				  Size tile_size) {
	Tiled *tiled_map = mem_zone_alloc(memory_pool, sizeof(Tiled));
	tiled_map->map_size = map_size;
	tiled_map->tile_size = tile_size;
	tiled_map->sprite = sprite;

	// allocate map
	tiled_map->map = mem_zone_alloc(memory_pool, map_size.width * map_size.height);
	memset(tiled_map->map, -1, map_size.width * map_size.height);

	// read file from dfs
	const char *tok;
	int fp = dfs_open(map_path);

	char *buffer = malloc(dfs_size(fp));
	int bytes_read;
	size_t i = 0;
	while ((bytes_read = dfs_read(buffer, sizeof(char), dfs_size(fp), fp)) > 0 &&
		   i < map_size.width * map_size.height) {
		for (tok = strtok(buffer, ","); tok && *tok; tok = strtok(NULL, ",\n\r")) {
			tiled_map->map[i] = (char)atoi(tok);
			++i;
		}
	}

	free(buffer);
	dfs_close(fp);

	return tiled_map;
}

// Render a Tiled map
void tiled_render(display_context_t disp, Tiled *tiled, Rect screen_rect) {
	for (size_t y = 0; y < tiled->map_size.height; y++) {
		for (size_t x = 0; x < tiled->map_size.width; x++) {
			size_t tile = (y * (int)tiled->map_size.width) + x;
			if (tiled->map[tile] == -1)
				continue;

			graphics_draw_sprite_trans_stride(disp, x * tiled->tile_size.width,
											  y * tiled->tile_size.height, tiled->sprite,
											  tiled->map[tile]);
		}
	}
}
