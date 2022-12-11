#include "../include/animated_sprite.h"

#include <math.h>

AnimatedSprite *animated_sprite_init(MemZone *memory_pool, sprite_t *sprite, Size size,
									 Position render_offset, size_t offset_start, size_t offset_end,
									 float anim_speed) {
	AnimatedSprite *anim = mem_zone_alloc(memory_pool, sizeof(AnimatedSprite));
	anim->sprite = sprite;
	anim->offset_start = offset_start;
	anim->offset_end = offset_end;
	anim->anim_speed = anim_speed;
	anim->size = size;
	anim->render_offset = render_offset;

	anim->_current_offset = offset_start;
	anim->_last_tick = get_ticks_ms();
	anim->_ticks = 0;

	return anim;
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
	
	tex_format_t format = sprite_get_format(anim->sprite);

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
			rdpq_tex_load_tlut(sprite_get_palette(anim->sprite), 0, 16);
			break;
		case FMT_CI8:
			rdpq_set_mode_copy(true);
			rdpq_mode_tlut(TLUT_RGBA16);
			rdpq_tex_load_tlut(sprite_get_palette(anim->sprite), 0, 256);
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

	if (is_intersecting(new_rect(pos, anim->size), screen_rect)) {

		surface_t spritesheet_surface = sprite_get_pixels(anim->sprite);

		int tex_width = anim->sprite->width / anim->sprite->hslices;
		int tex_height = anim->sprite->height / anim->sprite->hslices;

		int s_0 = ((int)anim->_current_offset % anim->sprite->hslices) * tex_width;
		int t_0 = ((int)anim->_current_offset / anim->sprite->hslices) * tex_height;
		int s_1 = s_0 + tex_width - 1;
		int t_1 = t_0 + tex_height - 1;

		rdpq_tex_load_sub(TILE0, &spritesheet_surface, 0, s_0, t_0, s_1, t_1);

		rdpq_texture_rectangle(TILE0, pos.x - anim->render_offset.x, pos.y - anim->render_offset.y,
			pos.x - anim->render_offset.x + tex_width, pos.y - anim->render_offset.y + tex_height,
			s_0, t_0, 1.f, 1.f);

	}
}
