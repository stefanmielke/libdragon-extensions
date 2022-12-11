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
			--tiled_map->map[i]; // subtract one from the index since Tiled starts at index 1 instead of zero
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

	tex_format_t format = sprite_get_format(tiled->sprite);

	// switch modes based on the appropriate texture format and
	// limitations. For example, 4 bit formats (excluding CI4) don't support copy mode

	switch(format){
		case FMT_I4:
		case FMT_I8:
			rdpq_set_mode_standard();
			break;
		case FMT_IA4:
			rdpq_set_mode_standard();
			rdpq_mode_alphacompare(ALPHACOMPARE_THRESHOLD);
			rdpq_set_blend_color(RGBA16(0,0,0,1));
			break;
		case FMT_CI4:
			rdpq_set_mode_copy(true);
			rdpq_mode_tlut(TLUT_RGBA16);
			rdpq_tex_load_tlut(sprite_get_palette(tiled->sprite), 0, 16);
			break;
		case FMT_CI8:
			rdpq_set_mode_copy(true);
			rdpq_mode_tlut(TLUT_RGBA16);
			rdpq_tex_load_tlut(sprite_get_palette(tiled->sprite), 0, 256);
			break;
		case FMT_IA8:
		case FMT_IA16:
		case FMT_RGBA16:
		case FMT_RGBA32:
			rdpq_set_mode_copy(true);
			break;
		default:
			return;
	}

	SET_VARS()

	int last_tile = -1;

	int tex_width;	// width of the parent tileset
	int tex_height;	// height of the parent tileset

	int s_0;	// initial s coordinate of tile
	int t_0;	// initial t coordinate of tile
	int s_1;	// final s coordinate of tile
	int t_1;	// final t coordinate of tile

	// initialize tile_surface to point to the pixels of the sprite's tileset
	surface_t tile_surface = sprite_get_pixels(tiled->sprite);

	BEGIN_LOOP()

	// if the tile has changed between this and last index, load it into TMEM
	if (last_tile != tiled->map[tile]) {

		last_tile = tiled->map[tile];

		tex_width = tiled->sprite->width / tiled->sprite->hslices;
		tex_height = tiled->sprite->height / tiled->sprite->hslices;

		s_0 = (tiled->map[tile] % tiled->sprite->hslices) * tex_width;
		t_0 = (tiled->map[tile] / tiled->sprite->hslices) * tex_height;
		s_1 = s_0 + tex_width - 1;
		t_1 = t_0 + tex_height - 1;
		
		rdpq_tex_load_sub(TILE0, &tile_surface, 0, s_0, t_0, s_0, t_1);
	}

	rdpq_texture_rectangle(TILE0, x * tiled->tile_size.width, y * tiled->tile_size.height,
								x * tiled->tile_size.width + tiled->tile_size.width,
								y * tiled->tile_size.height + tiled->tile_size.height, s_0, t_0, 1.f, 1.f);

	END_LOOP()

	rdpq_mode_tlut(TLUT_NONE);
	rdpq_mode_alphacompare(ALPHACOMPARE_NONE);
}

void tiled_destroy(Tiled *tiled) {
}
