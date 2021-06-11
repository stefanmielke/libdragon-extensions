#pragma once

#include <libdragon.h>
#include "mem_pool.h"
#include "util_defs.h"

typedef struct {
	sprite_t *sprite;
	size_t offset_start;
	size_t offset_end;
	/* rate of frame change in ms */
	float anim_speed;
	Size size;
	Position render_offset;

	float _current_offset;
	unsigned long _last_tick;
	unsigned long _ticks;
} AnimatedSprite;

AnimatedSprite *animated_sprite_init(MemZone *memory_pool, sprite_t *sprite, Size size,
									 Position render_offset, size_t offset_start, size_t offset_end,
									 float anim_speed);
void animated_sprite_tick(AnimatedSprite *anim, float anim_rate);
void animated_sprite_draw(AnimatedSprite *anim, Position pos, Rect screen_rect);
