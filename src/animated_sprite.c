#include "../include/animated_sprite.h"

#include <math.h>

AnimatedSprite *animated_sprite_init(MemZone *memory_pool, sprite_t *sprite, Size size,
									 Position render_offset, size_t offset_start, size_t offset_end,
									 float anim_speed) {
	AnimatedSprite *anim = mem_zone_alloc(memory_pool, sizeof(AnimatedSprite));
	animated_sprite_init_no_alloc(anim, sprite, size, render_offset, offset_start, offset_end,
								  anim_speed);

	return anim;
}

void animated_sprite_init_no_alloc(AnimatedSprite *anim, sprite_t *sprite, Size size,
								   Position render_offset, size_t offset_start, size_t offset_end,
								   float anim_speed) {
	anim->sprite = sprite;
	anim->offset_start = offset_start;
	anim->offset_end = offset_end;
	anim->anim_speed = anim_speed;
	anim->size = size;
	anim->render_offset = render_offset;

	anim->_current_offset = offset_start;
	anim->_last_tick = get_ticks_ms();
	anim->_ticks = 0;
}

void animated_sprite_tick(AnimatedSprite *anim, float anim_rate) {
	const size_t total_frames = anim->offset_end - anim->offset_start + 1;

	unsigned long current_tick = get_ticks_ms();
	unsigned long diff = current_tick - anim->_last_tick;
	anim->_last_tick = current_tick;

	anim->_ticks += diff * anim_rate;
	anim->_current_offset = (size_t)(anim->_ticks / anim->anim_speed) % total_frames;
	anim->_current_offset += anim->offset_start;
}

void animated_sprite_draw(AnimatedSprite *anim, Position pos, Rect screen_rect) {
	if (is_intersecting(new_rect(pos, anim->size), screen_rect)) {
		rdp_sync(SYNC_PIPE);
		rdp_load_texture_stride(0, 0, MIRROR_DISABLED, anim->sprite, anim->_current_offset);
		rdp_draw_sprite(0, pos.x - anim->render_offset.x, pos.y - anim->render_offset.y,
						MIRROR_DISABLED);
	}
}
