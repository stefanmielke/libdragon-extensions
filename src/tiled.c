#include "../include/tiled.h"

#include <string.h>

#define FILE_BUFFER_SIZE 100

#define SET_VARS()                                                                                 \
	size_t initial_x = screen_rect.pos.x / tiled->tile_size.width;                                 \
	size_t initial_y = screen_rect.pos.y / tiled->tile_size.height;                                \
	size_t final_x = ((screen_rect.pos.x + screen_rect.size.width) / tiled->tile_size.width) + 1;  \
	size_t final_y = ((screen_rect.pos.y + screen_rect.size.height) / tiled->tile_size.height) + 1;

#define BEGIN_LOOP()                                                                               \
	for (size_t y = initial_y; y < final_y; y++) {                                                 \
		for (size_t x = initial_x; x < final_x; x++) {                                             \
			size_t tile = (y * (int)tiled->map_size.width) + x;                                    \
			if (tiled->map[tile] == -1)                                                            \
				continue;

#define END_LOOP()                                                                                 \
	}                                                                                              \
	}

// Init a Tiled map
Tiled *tiled_init(MemZone *memory_pool, sprite_t *sprite, const char *map_path, Size map_size,
				  Size tile_size) {
	Tiled *tiled_map = NULL;
	if (memory_pool)
		tiled_map = mem_zone_alloc(memory_pool, sizeof(Tiled));
	else
		tiled_map = malloc(sizeof(Tiled));
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

void tiled_render(surface_t *disp, Tiled *tiled, Rect screen_rect) {
	SET_VARS()

	BEGIN_LOOP()

	graphics_draw_sprite_trans_stride(disp, x * tiled->tile_size.width, y * tiled->tile_size.height,
									  tiled->sprite, tiled->map[tile]);

	END_LOOP()
}

void tiled_render_rdp(Tiled *tiled, Rect screen_rect) {
	rdp_sync(SYNC_PIPE);
	SET_VARS()

	int last_tile = -1;

	BEGIN_LOOP()

	if (last_tile != tiled->map[tile]) {
		last_tile = tiled->map[tile];
		rdp_load_texture_stride(0, 0, MIRROR_DISABLED, tiled->sprite, tiled->map[tile]);
	}

	rdp_draw_textured_rectangle(0, x * tiled->tile_size.width, y * tiled->tile_size.height,
								x * tiled->tile_size.width + tiled->tile_size.width,
								y * tiled->tile_size.height + tiled->tile_size.height,
								MIRROR_DISABLED);

	END_LOOP()
}

void tiled_destroy(Tiled *tiled) {
}
