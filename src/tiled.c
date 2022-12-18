#include "../include/tiled.h"
#include "../include/format_handling.h"
#include "../include/csv_reader.h"
#include "../include/memory_alloc.h"

#include <string.h>

#define FILE_BUFFER_SIZE 100

#define CHECK_BOUNDS()                                                                             \
	if (!is_intersecting(screen_rect, tiled->map_rect))                                            \
		return;

#define SET_VARS()                                                                                 \
	int initial_x = (screen_rect.pos.x - tiled->offset.x) / tiled->tile_size.width;                \
	int initial_y = (screen_rect.pos.y - tiled->offset.y) / tiled->tile_size.height;               \
	size_t final_x = ((screen_rect.pos.x + screen_rect.size.width - tiled->offset.x) /             \
					  tiled->tile_size.width) +                                                    \
					 1;                                                                            \
	size_t final_y = ((screen_rect.pos.y + screen_rect.size.height - tiled->offset.y) /            \
					  tiled->tile_size.height) +                                                   \
					 1;                                                                            \
	if (initial_x < 0)                                                                             \
		initial_x = 0;                                                                             \
	if (initial_y < 0)                                                                             \
		initial_y = 0;                                                                             \
	if (final_x > tiled->map_size.width)                                                           \
		final_x = tiled->map_size.width;                                                           \
	if (final_y > tiled->map_size.height)                                                          \
		final_y = tiled->map_size.height;

#define BEGIN_LOOP()                                                                               \
	for (size_t y = initial_y; y < final_y; y++) {                                                 \
		for (size_t x = initial_x; x < final_x; x++) {                                             \
			size_t tile = (y * (int)tiled->map_size.width) + x;                                    \
			int screen_x = x * tiled->tile_size.width - screen_rect.pos.x + tiled->offset.x +      \
						   view_position.x;                                                        \
			int screen_y = y * tiled->tile_size.height - screen_rect.pos.y + tiled->offset.y +     \
						   view_position.y;                                                        \
			int screen_actual_x = screen_x;                                                        \
			int screen_actual_y = screen_y;                                                        \
			if (screen_actual_x < 0) {                                                             \
				screen_actual_x = 0;                                                               \
			}                                                                                      \
			if (screen_actual_y < 0) {                                                             \
				screen_actual_y = 0;                                                               \
			}                                                                                      \
			if (tiled->map[tile] == -1)                                                            \
				continue;

#define END_LOOP()                                                                                 \
	}                                                                                              \
	}

// Init a Tiled map
Tiled *tiled_init(MemZone *memory_pool, sprite_t *sprite, const char *map_path, Size map_size,
				  Size tile_size) {
	Tiled *tiled_map = MEM_ALLOC(sizeof(Tiled), memory_pool);
	tiled_map->map_size = map_size;
	tiled_map->tile_size = tile_size;
	tiled_map->sprite = sprite;

	tiled_map->offset = new_position_zero();
	tiled_map->map_rect = new_rect(tiled_map->offset, new_size(map_size.width * tile_size.width,
															   map_size.height * tile_size.height));

	// allocate map
	tiled_map->map = MEM_ALLOC(sizeof(int16_t) * map_size.width * map_size.height, memory_pool);
	memset(tiled_map->map, -1, sizeof(int16_t) * map_size.width * map_size.height);

	// read map from file
	csv_reader_from_ints(map_path, map_size.width * map_size.height, tiled_map->map);

	return tiled_map;
}

void tiled_set_render_offset(Tiled *tiled, Position offset) {
	tiled->offset = offset;
	tiled->map_rect.pos = offset;
}

void tiled_render(surface_t *disp, Tiled *tiled, Rect screen_rect, Position view_position) {
	CHECK_BOUNDS()

	SET_VARS()

	BEGIN_LOOP()

	graphics_draw_sprite_trans_stride(disp, screen_x, screen_y, tiled->sprite, tiled->map[tile]);

	END_LOOP()
}

void tiled_render_rdp(Tiled *tiled, Rect screen_rect, Position view_position) {
	format_set_render_mode(tiled->sprite, false);  // Configure the RDP render modes

	SET_VARS()

	int last_tile = -1;

	// The size of each tile
	SizeInt tex_size = new_sizeint(tiled->sprite->width / tiled->sprite->hslices,
								   tiled->sprite->height / tiled->sprite->vslices);
	PositionInt tex_coord_left =
		new_position_int_zero();  // The top left texture coordinate of the tile
	PositionInt tex_coord_right =
		new_position_int_zero();  // The bottom right texture coordinate of the tile

	// initialize tile_surface to point to the pixels of the sprite's tileset
	surface_t tile_surface = sprite_get_pixels(tiled->sprite);

	BEGIN_LOOP()

	// if the tile has changed between this and last index, load it into TMEM
	if (last_tile != tiled->map[tile]) {
		last_tile = tiled->map[tile];

		// configure the loaded region for this tile
		tex_coord_left.x = (tiled->map[tile] % tiled->sprite->hslices) * tex_size.width;
		tex_coord_left.y = (tiled->map[tile] / tiled->sprite->hslices) * tex_size.height;
		tex_coord_right.x = tex_coord_left.x + tex_size.width - 1;
		tex_coord_right.y = tex_coord_left.y + tex_size.height - 1;
		// load the tile region into TMEM
		rdpq_tex_load_sub(TILE0, &tile_surface, 0, tex_coord_left.x, tex_coord_left.y,
						  tex_coord_right.x, tex_coord_right.y);
	}

	rdpq_texture_rectangle(TILE0, screen_actual_x, screen_actual_y, screen_x + tex_size.width,
						   screen_y + tex_size.height,
						   tex_coord_left.x + screen_actual_x - screen_x,
						   tex_coord_left.y + screen_actual_y - screen_y, 1, 1);

	END_LOOP()

	rdpq_mode_tlut(TLUT_NONE);
	rdpq_mode_alphacompare(ALPHACOMPARE_NONE);
}

void tiled_destroy(Tiled *tiled) {
}
