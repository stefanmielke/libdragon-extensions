#include "../include/sprite_batch.h"
#include "../include/memory_alloc.h"

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
	rdp_sync(SYNC_PIPE);
	rdp_load_texture_stride(0, 0, MIRROR_DISABLED, sprite_batch->sprite, offset);

	Rect rect;
	rect.size = sprite_batch->size;
	for (size_t i = 0; i < sprite_batch->qty; ++i) {
		rect.pos = sprite_batch->positions[i];
		if (is_intersecting(rect, screen_rect)) {
			rdp_draw_sprite(0, rect.pos.x - sprite_batch->render_offset.x,
							rect.pos.y - sprite_batch->render_offset.y, MIRROR_DISABLED);
		}
	}
}

void sprite_batch_destroy(SpriteBatch *sprite_batch) {
	free(sprite_batch->positions);
	free(sprite_batch);
}
