#include "../include/font.h"

#include <spritesheet.h>

static struct SpriteFont {
	sprite_t *sprite;
	int tab_width;
	int font_width;
	int font_height;
} sprite_font;

static uint32_t f_color = 0xFFFFFFFF;
static uint32_t b_color = 0x00000000;

#define __get_buffer(x) __safe_buffer[(x)-1]
extern uint32_t __bitdepth;
extern uint32_t __width;
extern uint32_t __height;
extern void *__safe_buffer[];

#define __set_pixel(buffer, x, y, color) (buffer)[(x) + ((y)*__width)] = color

static int __is_transparent(int bitdepth, uint32_t color) {
	if (bitdepth == 2) {
		/* Is alpha bit set? */
		if ((color & 0x1) == 0x0) {
			return 1;
		}
	} else {
		/* Is alpha byte set? */
		if ((color & 0xFF) == 0x00) {
			return 1;
		}
	}

	return 0;
}

void graphics_set_font(sprite_t *font, int tab_width) {
	sprite_font.sprite = font;
	sprite_font.tab_width = tab_width;
	sprite_font.font_width = sprite_font.sprite->width / sprite_font.sprite->hslices;
	sprite_font.font_height = sprite_font.sprite->height / sprite_font.sprite->vslices;
}

void graphics_set_color_font(uint32_t forecolor, uint32_t backcolor) {
	f_color = forecolor;
	b_color = backcolor;
}

void graphics_draw_text_font(display_context_t disp, int x, int y, const char *const msg) {
	if (!disp || !msg || !sprite_font.sprite) {
		return;
	}

	int tx = x;
	int ty = y;
	const char *text = (const char *)msg;

	while (*text) {
		switch (*text) {
			case '\r':
			case '\n':
				tx = x;
				ty += sprite_font.font_height;
				break;
			case ' ':
				tx += sprite_font.font_width;
				break;
			case '\t':
				tx += sprite_font.font_width * sprite_font.tab_width;
				break;
			default:
				graphics_draw_character_font(disp, tx, ty, *text);
				tx += sprite_font.font_width;
				break;
		}

		text++;
	}
}

void graphics_draw_character_font(display_context_t disp, int x, int y, char ch) {
	if (disp == 0) {
		return;
	}

	int depth = __bitdepth;

	/* Figure out if they want the background to be transparent */
	int trans = __is_transparent(depth, b_color);

	const int sx = (ch % sprite_font.sprite->hslices) * sprite_font.font_width;
	const int sy = (ch / sprite_font.sprite->hslices) * sprite_font.font_height;
	const int ex = sx + sprite_font.font_width;
	const int ey = sy + sprite_font.font_height;

	const int tx = x - sx;
	const int ty = y - sy;

	if (depth == 2) {
		uint16_t *buffer = (uint16_t *)__get_buffer(disp);
		uint16_t *sp_data = (uint16_t *)sprite_font.sprite->data;

		for (int yp = sy; yp < ey; yp++) {
			const register int run = yp * sprite_font.sprite->width;

			for (int xp = sx; xp < ex; xp++) {
				const char c = sp_data[xp + run];
				if (trans) {
					if (c & 0x80) {
						__set_pixel(buffer, tx + xp, ty + yp, f_color);
					}
				} else {
					__set_pixel(buffer, tx + xp, ty + yp, (c & 0x80) ? f_color : b_color);
				}
			}
		}
	} else {
		uint32_t *buffer = (uint32_t *)__get_buffer(disp);
		uint32_t *sp_data = (uint32_t *)sprite_font.sprite->data;

		for (int yp = sy; yp < ey; yp++) {
			const register int run = yp * sprite_font.sprite->width;

			for (int xp = sx; xp < ex; xp++) {
				const char c = sp_data[xp + run];
				if (trans) {
					if (c & 0x80) {
						__set_pixel(buffer, tx + xp, ty + yp, f_color);
					}
				} else {
					__set_pixel(buffer, tx + xp, ty + yp, (c & 0x80) ? f_color : b_color);
				}
			}
		}
	}
}
