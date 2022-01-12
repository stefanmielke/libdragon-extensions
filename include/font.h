#pragma once

#include <libdragon.h>

/**
 * @brief Set the current font. Should be set before using any of the draw function.
 *
 * @param[in] font
 *        Sprite font to be used. Should be using a standard font sprite (containing every ASCII
 * character) and with a transparent background, and be loaded on mksprite with width and height
 * of 16.
 * @param[in] tab_width
 *        Width of each '\t' used. Is multiplied by the width of each character.
 */
void graphics_set_font(sprite_t *font, int tab_width);

/**
 * @brief Fonts to be used to draw. Equivalent of 'graphics_set_color', but for this font drawing.
 *
 * @param[in] forecolor
 *        Color used for the character.
 * @param[in] backcolor
 *        Color used for transparent pixels on the font sprite.
 */
void graphics_set_color_font(uint32_t forecolor, uint32_t backcolor);

/**
 * @brief Draw a text usign the font loaded with #graphics_set_font.
 *
 * @param[in] disp
 *            The currently active display context.
 * @param[in] x
 *            The X coordinate to place the top left pixel of the character drawn.
 * @param[in] y
 *            The Y coordinate to place the top left pixel of the character drawn.
 * @param[in] msg
 *            The ASCII null terminated string to draw to the screen.
 */
void graphics_draw_text_font(display_context_t disp, int x, int y, const char *const msg);

/**
 * @brief Draw a character to the screen using the built-in font
 *
 * Draw a character from the built-in font to the screen.  This function does not support alpha
 * blending, only binary transparency.  If the background color is fully transparent, the font is
 * drawn with no background.  Otherwise, the font is drawn on a fully colored background.  The
 * foreground and background can be set using #graphics_set_color_font.
 *
 * @param[in] disp
 *            The currently active display context.
 * @param[in] x
 *            The X coordinate to place the top left pixel of the character drawn.
 * @param[in] y
 *            The Y coordinate to place the top left pixel of the character drawn.
 * @param[in] ch
 *            The ASCII character to draw to the screen.
 */
void graphics_draw_character_font(display_context_t disp, int x, int y, char ch);
