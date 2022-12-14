#include "../include/sprite_batch.h"
#include "../include/memory_alloc.h"
#include "../include/format_handling.h"

SpriteBatch *sprite_batch_init(MemZone *memory_pool, sprite_t *sprite, size_t qty, Size size,
							   Position render_offset) {
	SpriteBatch *batch = MEM_ALLOC(sizeof(SpriteBatch), memory_pool);
	batch->positions = MEM_ALLOC(sizeof(Position) * qty, memory_pool);
	batch->sprite = sprite;
	batch->qty = qty;
	batch->size = size;
	batch->render_offset = render_offset;

	return batch;
}

void sprite_batch_draw(SpriteBatch *sprite_batch, int offset, Rect screen_rect) {
	
	format_set_render_mode(sprite_batch->sprite, false);

	int tex_width = sprite_batch->sprite->width / sprite_batch->sprite->hslices;
	int tex_height = sprite_batch->sprite->height / sprite_batch->sprite->vslices;
	
	int s_0 = (offset % sprite_batch->sprite->hslices) * tex_width;
	int t_0 = (offset / sprite_batch->sprite->hslices) * tex_height;
	int s_1 = s_0 + tex_width - 1;
	int t_1 = t_0 + tex_height - 1;

	surface_t sprite_surf = sprite_get_pixels(sprite_batch->sprite);

	rdpq_tex_load_sub(TILE0, &sprite_surf, 0, s_0, t_0, s_1, t_1);

	Rect rect;
	rect.size = sprite_batch->size;
	for (size_t i = 0; i < sprite_batch->qty; ++i) {
		rect.pos = sprite_batch->positions[i];
		if (is_intersecting(rect, screen_rect)) {

			int x_0 = rect.pos.x - sprite_batch->render_offset.x;
			int y_0 = rect.pos.y - sprite_batch->render_offset.y;

			rdpq_texture_rectangle(TILE0, x_0, y_0, x_0 + tex_width, y_0 + tex_width, s_0, t_0, 1, 1);
		}
	}
}

void sprite_batch_destroy(SpriteBatch *sprite_batch) {
	free(sprite_batch->positions);
	free(sprite_batch);
}
