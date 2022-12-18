#pragma once

#include <libdragon.h>
#include "mem_pool.h"
#include "rect.h"
#include "position_int.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Struct the positions of a single tile type.
 */
typedef struct {
	size_t count;
	PositionInt *position;
} TiledCachedTile;

/**
 * @brief Struct that holds a TiledCached map.
 */
typedef struct {
	/// All tiles cached. The index is the tile as it is in the CSV file.
	TiledCachedTile tiles[255];
	/// Size of the map in tiles.
	Size map_size;
	/// Size of a tile.
	Size tile_size;
	/// Sprite used to render the tiles.
	sprite_t *sprite;
} TiledCached;

/**
 * @brief Allocates, loads, and initializes the TiledCached map.
 *
 * @param memory_pool
 *        MemZone to use to allocate. If NULL will use malloc instead, call 'tiled_cached_destroy'
 * if you do.
 * @param sprite
 *        Sprite used to render.
 * @param map_path
 *        Path to the map file (eg.: "/maps/my_map.csv"). Has to be a CSV file.
 * @param map_size
 *        Size of the map in tiles.
 * @param tile_size
 *        Size of each tile.
 *
 * @return The new TiledCached.
 */
__attribute__((deprecated("TiledCached is no longer actively supported"))) TiledCached *
tiled_cached_init(MemZone *memory_pool, sprite_t *sprite, const char *map_path, Size map_size,
				  Size tile_size);

/**
 * @brief Render a Tiled map using hardware rendering. Use this method when there aren't that many
 * different tiles, but they are more spread out. See 'Tiled' for other use-cases.
 *
 * @param tiled
 *        TiledCached to render.
 * @param screen_rect
 *        Rect of the current screen. Used to cull tiles outside of the screen.
 */
__attribute__((deprecated("TiledCached is no longer actively supported"))) void tiled_cached_render(
	TiledCached *tiled, Rect screen_rect);

/**
 * @brief Destroy a TiledCached that didn't use a memory pool on init.
 *
 * @param tiled
 *        TiledCached to destroy.
 */
__attribute__((deprecated("TiledCached is no longer actively supported"))) void
tiled_cached_destroy(TiledCached *tiled);

#ifdef __cplusplus
}
#endif