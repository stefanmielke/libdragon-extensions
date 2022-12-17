# Libdragon Extensions
List of structs and functions to help on Libdragon development.

## Installation

You can either [download the code from GitHub](https://github.com/stefanmielke/libdragon-extensions/archive/refs/heads/main.zip) into your project, or [clone the repo](https://github.com/stefanmielke/libdragon-extensions.git) inside your project as a [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) (so you can use `git pull` to get the latest changes when needed).

## Extensions Available

- [Position](#Position)
- [PositionInt](#PositionInt)
- [Size](#Size)
- [Rect](#Rect)
- [Random](#Random)
- [Range](#Range)
- [Spritesheet Support](#Spritesheet-Support)
- [Color Support](#Color-Support)
- [Sprite Batch](#Sprite-Batch)
- [Animated Sprite](#Animated-Sprite)
- [Tiled Support](#Tiled-Support)
- [Scene Manager](#Scene-Manager)
- [Object Pooling (Free List)](#Object-Pooling-Free-List)
- [Clock/Timer](#ClockTimer)
- [Menu](#Menu)
- [Tweening](#Tweening)
- [Render Mode Configuration](#Format-Handling)

### Position
> position.h

```c
typedef struct {
	float x;
	float y;
} Position;

Position new_position_zero();
Position new_position_same(float x_and_y);
Position new_position(float x, float y);
```

### PositionInt
> position_int.h

```c
typedef struct {
	int x;
	int y;
} PositionInt;

Position new_position_int_zero();
Position new_position_int_same(int x_and_y);
Position new_position_int(int x, int y);
```

### Size
>size.h

```c
typedef struct {
	float width;
	float height;
} Size;

Size new_size_zero();
Size new_size_same(float width_and_height);
Size new_size(float width, float height);
```

### Rect
> rect.h

```c
typedef struct {
	Position pos;
	Size size;
} Rect;

Rect new_rect(Position pos, Size size);
bool is_intersecting(Rect a, Rect b);
bool contains(Rect inner, Rect outer);
```

### Random
> random.h

Set of functions to simplify random numbers in a range.

```c
// generates an int from 0 to 10 (inclusive)
int n1 = RANDN(10);
// generates an int from 10 to 20 (inclusive)
int n2 = RANDR(10, 20);
// generates a float from 0 to 10 (inclusive)
float f1 = RANDNF(10);
// generates an int from 10 to 20 (inclusive)
float f2 = RANDRF(10, 20)
```

### Range
> range.h

Struct that contains a range and has functions to get random values from it.

```c
// generates a range from 0 to 10
RangeFloat r1 = new_range_float(0, 10);
// generates a range from 10 to 10
RangeFloat r2 = new_range_float_same(10);

// gets a random value from 0 to 10
float value = range_get_from_float(&r1);
```

### Spritesheet Support
> spritesheet.h

Libdragon supports spritesheet out of the box, so here we only have support methods to ease loading them (and to use our memory pool).

```c
// load spritesheet using a memory pool
sprite_t *my_sprite = spritesheet_load(&memory_pool, "/path/to/sprite.sprite");

// load spritesheet with no memory pool
sprite_t *my_sprite = spritesheet_load(NULL, "/path/to/sprite.sprite");
// remember to free the sprite if not using a memory pool!
free(my_sprite);
```

### Color Support
> color.h

Libdragon doesn't provide a single function that generates a color_t. So I created this function for this.

```c
color_t color = new_color(r, g, b, a);
```

### Sprite Batch
> sprite_batch.h | sprite_batch.c

Used to quickly draw the same sprite on multiple positions at the same time.

```c
// initialize
size_t batch_size = 10;
Size sprite_size = new_size_same(10);
Position offset_position = new_position_same(7); // start drawing at position - 7, the sprite is bigger than its collision
SpriteBatch *batch = sprite_batch_init(&memory_pool, sprites, batch_size, sprite_size, offset_position);
// you can pass NULL on memory_pool to use malloc instead of MemZone.
SpriteBatch *batch = sprite_batch_init(NULL, sprites, batch_size, sprite_size, offset_position);

// set positions
for (size_t i = 0; i < batch_size; ++i) {
    batch->positions[i].x = i * 2;
    batch->positions[i].y = i * 2;
}

// draw sprites
Rect screen_rect = new_rect(new_pos(0, 0), new_size(320, 240));
int current_frame = 5; // we can update the frame that will be used on all draw calls
sprite_batch_draw(batch, current_frame, screen_rect);

// destroy the SpriteBatch. Should only be called if you sent NULL on 'memory_pool' when initializing.
sprite_batch_destroy(batch);
```

### Animated Sprite
> animated_sprite.h | animated_sprite.c

Used to animate and draw a sprite.

```c
// initialize
float anim_speed = 100; // how many ms to wait for a frame to change
size_t first_frame = 0, last_frame = 4;
AnimatedSprite *anim =
    animated_sprite_init(&memory_pool, sprites, new_size_zero(), new_position_zero(), first_frame, last_frame, anim_speed);

// tick
float anim_rate = 1; // how fast the animation should run on this tick
animated_sprite_tick(anim, anim_rate);

// draw sprite on position (10,10)
// screen_rect is used to check boundaries
Rect screen_rect = new_rect(new_pos(0, 0), new_size(320, 240));
animated_sprite_draw(anim, new_pos(10, 10), screen_rect);
```

### Memory Pool
> mem_pool.h | mem_pool.c

Simple memory pool to allocate/free memory during gameplay without causing malloc (slower) calls.

```c
MemZone memory_pool;

// allocating 1KB to be used
mem_zone_init(&memory_pool, 1 * 1024);

// gets one int from the pool
int value = mem_zone_alloc(&memory_pool, sizeof(int));
value = 1;

// resets pool to the beggining
mem_zone_free_all(&memory_pool);

// gets another int, but on the same memory space as before
// if no value is set it will be the same set previously (on this example, '1')
int value2 = mem_zone_alloc(&memory_pool, sizeof(int));
```

### Tiled Support

Tiled support is still not as performant due to the way the N64 works and how Libdragon works, but can work for fewer tiles and textures.

It has support for Tiled CSV files, and you can use those directly, just make sure you import it using `mkdfs`.

It comes in two variants.

> tiled.h | tiled.c

This variant is the simple one, rendering left to right / top to bottom, can use software and hardware rendering according to your needs, and has culling.

The software renderer is recommended for maps with lots of texture swaps during rendering, as it provides constant time to render.

The hardware rendering is preffered when there's not much texture swapping and can render maps faster than the software renderer if that's the case.

The maximum map size I was able to load was 50x50, so take that into consideration as well.

```c
// load the map
Size grid_size = new_size(50, 50); // map tiles: 50x50
Size tile_size = new_size_same(16); // tile size: 16x16
Tiled *tile_test = tiled_init(&memory_pool, tile_sprite, "/path/to/map.map", grid_size, tile_size);
// using malloc instead of memory pool
Tiled *tile_test = tiled_init(NULL, tile_sprite, "/path/to/map.map", grid_size, tile_size);

// Render the map (software renderer)
tiled_render(disp, tile_test, screen_rect);

// Render the map (hardware renderer)
tiled_render_rdp(tile_test, screen_rect);

// if not using memory pool (and only if not using), you have to call destroy to free the memory used
tiled_destroy(tile_test);
```

> tiled_cached.h | tiled_cached.c

This variant can perform really well even if you have more than a few different textures, due to its caching, that tries to prevent texture swaps as much as it can.

It will consume more memory, and will take more time to init, and also doesn't have culling implemented.

```c
// load the map
Size grid_size = new_size(50, 50); // map tiles: 50x50
Size tile_size = new_size_same(16); // tile size: 16x16
TiledCached *tile_test = tiled_cached_init(&memory_pool, tile_sprite, "/path/to/map.map", grid_size, tile_size);
// using malloc instead of memory pool
TiledCached *tile_test = tiled_cached_init(NULL, tile_sprite, "/path/to/map.map", grid_size, tile_size);

// Render the map
tiled_cached_render(tile_test, screen_rect);

// if not using memory pool (and only if not using), you have to call destroy to free the memory used
tiled_cached_destroy(tile_test);
```

### Scene Manager

You can use this to manage the transition across different scenes (aka levels).

This allows you to create different files that can represent your scenes and just create callbacks that will be used by the Scene Manager.

```c
// callbacks for a scene
void main_screen_create();
short main_screen_tick();
void main_screen_display(surface_t *disp);
void main_screen_destroy()

// callback for changing scenes
enum screens {
	SCREEN_MAIN
};
void change_screen(short curr_screen, short next_screen) {
	switch (next_screen) {
		case SCREEN_MAIN:
			scene_manager_set_callbacks(scene_manager, &main_screen_create, &main_screen_tick,
										&main_screen_display, &main_screen_destroy);
			break;
		default:
			abort();
	}
}

// 3 ways to initialize scene manager

// 1. I have a global and scene memory pools
SceneManager *scene_manager = scene_manager_init(&global_memory_pool, &memory_pool, &change_screen);
// 2. I use malloc or global vars for globals
SceneManager *scene_manager = scene_manager_init(NULL, &memory_pool, &change_screen);
// 3. I want to manage my memory pools myself
SceneManager *scene_manager = scene_manager_init(NULL, NULL, &change_screen);

// set up first screen after initializing
scene_manager_change_scene(scene_manager, SCREEN_MAIN);

// call tick every frame after controller_scan
scene_manager_tick(scene_manager);

// call every tick to draw to the screen after display_lock
scene_manager_display(scene_manager, disp);

// you can call this method at any time during screen tick to set up screen change
// it will only really change the scene at the beggining of the next tick
scene_manager_change_scene(scene_manager, SCREEN_PLAY);

// you can check if the game will change on the next tick using this check after `scene_manager_tick`
if (scene_manager->current_scene_id != scene_manager->next_scene_id) {
	// Screen will change! Draw loading screen like below instead of calling `scene_manager_display`?
	graphics_fill_screen(disp, BLACK);
	graphics_set_color(WHITE, BLACK);
	graphics_draw_text(disp, 0, 0, "Loading...");
}

// destroy the scene manager if not using a global memory pool
scene_manager_destroy(scene_manager);
```

### Object Pooling (Free List)

Object Pooling is used to create and dispose of objects rapidly and without causing memory fragmentation.

It uses a macro to ensure that you can use your struct without having to cast from and to void*, but that can lead to more code being generated, so use it with care.

The implementation provided is of a [Free List](https://gameprogrammingpatterns.com/object-pool.html#a-free-list) and based off of [djolman's implementation](https://github.com/djoldman/fmpool) with a few modifications.

```c
// define the struct to be used
typedef struct
{
	int x;
	int y;
} Point_t;

// initialize the Object Pool macro to use 'Point_t'
// this will create all the struct and function definitions used in the implementation
// remember that you can initialize more than once and it will generate one for each struct
OBJPOOL_INIT(Point_t)

// creates the object pool
objpool_t(Point_t) * pool;
// when using a memory pool
pool = objpool_create(Point_t, &memory_pool, 10);
// if not using a memory pool. remember to call 'objpool_destroy' later in this case.
pool = objpool_create(Point_t, NULL, 10);

// Get a new object from the pool
Point_t *new_obj = objpool_get(Point_t, pool);

// Free the object from the pool
objpool_free(Point_t, pool, new_obj);

// Free pool's memory. Only call this if not using a memory pool.
objpool_destroy(Point_t, pool);
```

### Clock/Timer
> clock.h

Simple clock that enables a function to be called on fixed intervals. Can also be paused and resumed at any time.

```c
// function that will be called every X seconds
void function_callback();

Clock *clock;

// initialize the clock that will be called every second
clock = new_clock(&mem_pool, 1000, &function_callback); // with a memory pool
clock = new_clock(NULL, 1000, &function_callback); // without a memory pool

// every frame we tick it
clock_tick(clock);

// we can pause the clock, and it will not do anything when you call tick
clock_pause(clock);
// and also resume and it will continue from when it paused (will not call any 'missed' callbacks)
clock_resume(clock);

// frees the memory allocated on 'new_clock'. DO NOT call if using a memory pool
clock_destroy(clock);
```

### Menu
> menu.h

Supports a simple menu with background rendering, scrolling, and nested sub-menus. Uses basic text rendering.

The interaction with the menu occurs by using 'A' or 'START' to select and 'B' to exit a sub-menu.

To understand the basic texture used, see `sample_assets/menu.png`.

```c
// -- initializing global variables used by every menu --

// main init function
// (required to set default colors, can omit if setting colors using `menu_global_set_default_colors` or setting colors for items)
menu_global_init();

// initializes the sprites and offsets used. only required if using them (when creating the menu)
menu_global_set_sprites(menu_sprite, hand_sprite, hand_sprite_offset);

// optionally overriding the default colors
uint32_t selected = graphics_make_color(0, 0, 0, 255);
//... ommiting the other colors, but you can use the function above to set each color
menu_global_set_default_colors(selected, enabled, disabled, background, out_of_bounds, menu_background);

// -- now to create the menu... --

// callback to be used. it's optional, but improves code legibility when using sub-menus.
void menu_callback(int option, MenuItem* item);

Menu* menu;
// create a new menu (memory_pool can be NULL if not using it)
uint8_t total_items = 10; // max items that the menu can have
uint8_t max_items = 5; // max items that will be displayed at any time (if more than this, there will be scrolling)
int top = 100, left = 100; // positions on-screen of the menu
uint8_t item_height = 16; // height in pixels that each item will have (used to calculate background bottom and scrolling)
menu = menu_init(&memory_pool, total_items, max_items, top, left, item_height, NULL); // not using callbacks
menu = menu_init(&memory_pool, total_items, max_items, top, left, item_height, &menu_callback); // using callbacks

// add items
bool enabled = true; // if the item can be selected or not. color can differ baed on colors set.
void *object = &my_custom_object; // this can be held to be retrieved later for custom actions baed on selected item.

// adding simple text items
menu_add_item(menu, "Item 1", enabled, object); // with object
menu_add_item(menu, "Item 2", false, NULL); // no object here, forcing disabled

// adding colored item (can use different colors for each item)
uint32_t color_selected = graphics_make_color(0, 0, 0, 255);
// omitting other colors...
menu_add_item_colored(menu, "Item 3", enabled, color_selected, color_enabled, color_disabled, object);

// adding item as an image instead of text
sprite_t *custom_image_sprite; // custom sprite to be used by the item
int sprite_index = 2; // index for the custom sprite
menu_add_item_image(menu, custom_image_sprite, sprite_index, enabled, object);

// ticks the menu to calculate changes to its state. should only be called when the menu is active.
struct controller_data keys_down = get_keys_down(); // you should do this only once per frame in your normal code
int selected_option = menu_tick(menu, &keys_down);
// if 'selected_option >= 0' then the corresponding item was selected and you can act accordingly.
// if using callbacks, 'selected_option' can also be '>= 0', but the function will also be called.

// render the menu
menu_render(menu, disp);

// -- optional set-up steps: --

// add hand cursor
int hand_position_x = 105; // X position of the hand. this will never change.
int hand_position_y_offset = 5; // offset in pixels to be applied to the sprite when rendering the hand.
menu_set_hand(menu, hand_position_x, hand_position_y_offset);

// add background
int menu_width = 200; // width in pixels of the menu. this will be the same regardless of the item width.
menu_set_background(menu, menu_width);
```

**Sub-menus**

Can be used to support N amount of nested menus without losing state on the parent menus. They are automatically ticked when ticking the parent, but the state is not set automatically (eg.: going from one menu to the other).

```c
// initialize the amount of sub-menus used (memory pool can be NULL if not using)
uint8_t total_submenus = 2; // amount of sub-menus that will be used (maximum)
bool display_when_on_submenu = false; // do not render parent menu if children is selected.
menu_init_submenus(menu, &memory_pool, total_submenus, display_when_on_submenu);

// initialize 2 additional menus (skipping adding items on this example)
Menu *menu2 = menu_init(&memory_pool, total_items, max_items, top, left, item_height, &callback_sub1);
Menu *menu3 = menu_init(&memory_pool, total_items, max_items, top, left, item_height, &callback_sub2);

// setting menus on the parent
menu->submenus[0] = menu2;
menu->submenus[1] = menu3;

// setting a sub-menu
menu->active_submenu = 0; // sets the active menu as 'menu2'.
menu->active_submenu = -1; // sets the parent menu ('menu') as the active.
```

**Examples**

Practical usage of tick with sub-menus:
```c
// not using callbacks
int option = menu_tick(menu, &keys_down);
if (option >= 0) {
	if (menu->active_submenu == -1) {
		// is currently on main menu, just 2 submenus
		switch (option) {
			case 0:
			case 1:
				// setting the sub-menu according to the chosen item
				menu->active_submenu = option;
				break;
			default:
				break;
		}
	} else if (menu->active_submenu == 0) {
		// is currently on first sub-menu
		MenuItem *menu_item = &menu->submenus[0]->items[option];
		MyStruct *item = menu_item->object;
		// do something with the selected item
	} else if (menu->active_submenu == 1) {
		// is currently on second sub-menu
		MenuItem *menu_item = &menu->submenus[1]->items[option];
		MyStruct *item = menu_item->object;
		// do something with the selected item
	}
}

// using callbacks
menu_tick(menu, &keys_down); // on your tick function

void game_screen_menu_callback(int option, MenuItem *item) {
	game_data->menu->active_submenu = option;
}

void game_screen_menu_decree_callback(int option, MenuItem *item) {
	MyStruct *object = item->object;
	// do something with the selected item
}

void game_screen_menu_buildings_callback(int option, MenuItem *item) {
	MyStruct *object = item->object;
	// do something with the selected item
}
```

How to use the custom object?
```c
// getting the current option (can also be the result of 'menu_tick')
int selected_item = menu->current_menu_option;
// get the object stored for that MenuItem
MyStruct *object = menu->items[selected_item].object;
```

### Tweening

We support [Tweening](https://en.wikipedia.org/wiki/Inbetweening) and easing through a few functions. Easing functions were based on [MonoGame.Extended Methods](https://www.monogameextended.net/docs/features/tweening/tweening#in-easing-functions).

```c
// example struct
typedef struct {
	int x;
} Player;
Player *my_player;
// callback for value changes
void tween_player_x(void *target_object, float current_value) {
	Player *player = (Player *)target_object;
	player->x = current_value;
}
void tween_player_size(void *target_object, Size current_value);
void tween_player_position(void *target_object, Position current_value);
void tween_player_color(void *target_object, uint32_t current_value);
// example callback for when the tween finishes
void tween_finished_player_x(void *target_object);

Tween *tween;

// initialize the tween with a memory pool
tween = tween_init(&memory_pool);
// initialize without a memory pool
tween = tween_init(NULL);

// starting the tween, examples
uint64_t duration_in_ms = 2000; // 2 seconds

// 1. No ending callback, no reverse, no always repeat
tween_start(tween, (void *)my_player, &easing_bounce_in_out, duration_in_ms, NULL, false, false);
// 2. Ending callback, no reverse, no always repeat
tween_start(tween, (void *)my_player, &easing_bounce_in_out, duration_in_ms, &tween_finished_player_x, false, false);
// 3. Ending callback, auto reverse, no always repeat
tween_start(tween, (void *)my_player, &easing_bounce_in_out, duration_in_ms, &tween_finished_player_x, true, false);
// 4. No ending callback, no auto reverse, always repeat
tween_start(tween, (void *)my_player, &easing_bounce_in_out, duration_in_ms, NULL, false, true);
// 5. No ending callback, auto reverse, always repeat
tween_start(tween, (void *)my_player, &easing_bounce_in_out, duration_in_ms, NULL, true, true);

// set the tween to the desired type
// float tween 
float start_x = 50, end_x = 200;
tween_set_to_float(tween, start_x, end_x, &tween_player_x);
// size tween
Size start_size = new_size_same(1), end_size = new_size_same(4);
tween_set_to_size(tween, start_size, end_size, &tween_player_size);
// position tween
Position start_position = new_position(10, 100), end_position = new_position(10, 300);
tween_set_to_position(tween, start_position, end_position, &tween_player_position);
// color tween (example uses new_color() from 'color.h')
color_t start_color = new_color(10, 100), end_color = new_color(10, 300);
tween_set_to_color(tween, start_color, end_color, &tween_player_color);

// calling tick every frame. It will call 'tween_player_x' with the new value, and 'tween_finished_player_x' when it finishes
tween_tick(tween);

// destroying the tween (unnecessary but safe when using a memory pool)
tween_destroy(tween);
```
### Render Mode Configuration

>format_handling.h

Using different texture formats and render features like mirroring usually requires configuring the RDP mode manually to make the best of performance. This provides an easy way for the user to automatically configure the RDP based on the input texture format.

```c
// Configure based on the format of sprite with mirroring disabled
format_set_render_mode(sprite, false);

// Render code goes here:
```

## More Examples

I'm trying to test and use this code on [my game](https://github.com/stefanmielke/mielkesparty_n64). So feel free to look there for more examples.

## Development

Make sure to use the included clang-format before pushing your code.

**Pull Requests are welcome!**
