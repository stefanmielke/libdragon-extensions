#pragma once

#include <libdragon.h>

#include "mem_pool.h"

typedef enum MenuBackgroundSprite {
	SPRITE_menu_bottom,
	SPRITE_menu_bottom_left,
	SPRITE_menu_bottom_right,
	SPRITE_menu_center,
	SPRITE_menu_left,
	SPRITE_menu_right,
	SPRITE_menu_top,
	SPRITE_menu_top_left,
	SPRITE_menu_top_right,
	SPRITE_menu_SPRITES_MAX
} MenuBackgroundSprite;

typedef enum MenuItemState {
	MSIS_Inside,
	MSIS_OnBorder,
	MSIS_Outside,
} MenuItemState;

typedef struct MenuItem {
	char *text;
	sprite_t *sprite;
	int sprite_index;

	int x;
	int y;
	bool enabled;
	bool has_custom_colors;
	uint32_t color_enabled;
	uint32_t color_disabled;
	uint32_t color_selected;

	MenuItemState state;

	void *object;
} MenuItem;

typedef struct Menu {
	MenuItem *items;
	uint8_t total_items;
	uint8_t current_add_index;
	int current_menu_option;

	int active_submenu;
	struct Menu **submenus;

	bool display_when_on_submenu;

	bool display_background;
	int menu_width;

	int hand_position_x;
	int hand_position_y_offset;

	uint8_t item_vertical_limit;
	uint8_t item_height;
	int top;
	int left;
	uint8_t cur_top_item;
	uint8_t cur_bottom_item;
} Menu;

Menu *menu_init(MemZone *memory_pool, uint8_t total_items, uint8_t max_items, int top, int left,
				uint8_t item_height);
void menu_set_background(Menu *menu, int menu_width);
void menu_reset_items(Menu *menu);
void menu_set_hand(Menu *menu, int hand_position_x, int hand_position_y_offset);
int menu_tick(Menu *menu, struct controller_data *keys_down);
void menu_add_item(Menu *menu, char *text, bool enabled, void *object);
void menu_add_item_image(Menu *menu, sprite_t *sprite, int sprite_index, bool enabled,
						 void *object);
void menu_add_item_colored(Menu *menu, char *text, bool enabled, uint32_t color_selected,
						   uint32_t color_enabled, uint32_t color_disabled, void *object);
void menu_render(Menu *menu, display_context_t disp);

void menu_init_submenus(Menu *menu, MemZone *memory_pool, uint8_t total_submenus,
						bool display_when_on_submenu);

void menu_draw_background_borders(display_context_t disp, int top, int left, int bottom, int right);
void menu_draw_background_center(display_context_t disp, int top, int left, int bottom, int right);

/**
 * @brief Initializes the menu default colors
 */
void menu_global_init();

/**
 * @brief Set the sprite used by the background menu
 *
 * @param menu_sprite
 *        Sprite that will be used. See `sample_assets/menu.png` for example.
 *        Order should be according to enum `MenuBackgroundSprite`.
 * @param hand_sprite
 *        Sprite that will be used as the 'hand'.
 * @param hand_sprite_offset
 *        Offset of the sprite (used by `graphics_draw_sprite_trans_stride`).
 */
void menu_global_set_sprites(sprite_t *menu_sprite, sprite_t *hand_sprite,
							 uint8_t hand_sprite_offset);

/**
 * @brief Set the default colors for all menus when the item doesn't have any set.
 *
 * @param selected
 *        Color used when the item is the current selected one. Default is {0,0,255,255}
 * @param enabled
 *        Color used when the item is enabled but not selected. Default is {255,255,255,255}
 * @param disabled
 *        Color used when the item is disabled. Default is {100,100,255,255}
 * @param background
 *        Color used for the background of the text. Default is {0,0,0,0}
 * @param out_of_bounds
 *        Color used when the item is out of the bounds of the menu (when scrolling). Default is
 * {0,0,0,255}
 * @param menu_background
 *        Color used for the background. Due to 32 bits issue, we do not use the background from the
 * sprite.
 */
void menu_global_set_default_colors(uint32_t selected, uint32_t enabled, uint32_t disabled,
									uint32_t background, uint32_t out_of_bounds,
									uint32_t menu_background);
