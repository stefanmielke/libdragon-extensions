#include "../include/tiled_cached.h"

#include <string.h>

TiledCached *tiled_cached_init(MemZone *memory_pool, sprite_t *sprite, const char *map_path,
							   Size map_size, Size tile_size) {
	TiledCached *tiled_map = mem_zone_alloc(memory_pool, sizeof(TiledCached));
	tiled_map->map_size = map_size;
	tiled_map->tile_size = tile_size;
	tiled_map->sprite = sprite;

	// allocate map
	char *map = malloc(map_size.width * map_size.height);
	memset(map, -1, map_size.width * map_size.height);

	// read file from dfs
	const char *tok;
	int fp = dfs_open(map_path);

	char *buffer = malloc(dfs_size(fp));
	int bytes_read;
	size_t i = 0;
	while ((bytes_read = dfs_read(buffer, sizeof(char), dfs_size(fp), fp)) > 0 &&
		   i < map_size.width * map_size.height) {
		for (tok = strtok(buffer, ","); tok && *tok; tok = strtok(NULL, ",\n\r")) {
			map[i] = (char)atoi(tok);
			++i;
		}
	}

	free(buffer);
	dfs_close(fp);

	// naive lookup
	for (size_t i = 0; i < 255; ++i) {
		size_t counter = 0;
		for (size_t y = 0; y < map_size.height; y++) {
			for (size_t x = 0; x < map_size.width; x++) {
				size_t tile = (y * (int)map_size.width) + x;
				if (map[tile] == i)
					++counter;
			}
		}
		tiled_map->tiles[i].count = counter;
		if (counter > 0) {
			tiled_map->tiles[i].position = malloc(sizeof(PositionInt) * counter);
			memset(tiled_map->tiles[i].position, 0, sizeof(PositionInt) * counter);

			tiled_map->tiles[i].position[0].x = 0;
			size_t curr = 0;
			for (int y = 0; y < map_size.height; y++) {
				for (int x = 0; x < map_size.width; x++) {
					size_t tile = (y * (int)map_size.width) + x;
					if (map[tile] == i) {
						tiled_map->tiles[i].position[curr].x = x;
						tiled_map->tiles[i].position[curr].y = y;
						++curr;
					}
				}
			}
		}
	}

	free(map);

	return tiled_map;
}

// Render a Tiled map
void tiled_cached_render(TiledCached *tiled, Rect screen_rect) {
	rdp_sync(SYNC_PIPE);

	for (size_t i = 0; i < 255; ++i) {
		if (tiled->tiles[i].count > 0) {
			rdp_load_texture_stride(0, 0, MIRROR_DISABLED, tiled->sprite, i);
			for (size_t j = 0; j < tiled->tiles[i].count; ++j) {
				rdp_draw_textured_rectangle(
					0, tiled->tiles[i].position[j].x * tiled->tile_size.width,
					tiled->tiles[i].position[j].y * tiled->tile_size.height,
					tiled->tiles[i].position[j].x * tiled->tile_size.width + tiled->tile_size.width,
					tiled->tiles[i].position[j].y * tiled->tile_size.height +
						tiled->tile_size.height,
					MIRROR_DISABLED);
			}
		}
	}
}