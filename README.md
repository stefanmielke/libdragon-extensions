# Libdragon Extensions
List of structs and functions to help on Libdragon development.

## Extensions Available

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

### Sprite Batch
> sprite_batch.h | sprite_batch.c

Used to quickly draw the same sprite on multiple positions at the same time.

```c
// initialize
size_t batch_size = 10;
Size sprite_size = new_size_same(10);
Position offset_position = new_position_same(7); // start drawing at position - 7, the sprite is bigger than its collision
SpriteBatch *batch = sprite_batch_init(&memory_pool, sprites, batch_size, sprite_size, offset_position);

// set positions
for (size_t i = 0; i < batch_size; ++i) {
    batch->positions[i].x = i * 2;
    batch->positions[i].y = i * 2;
}

// draw sprites
Rect screen_rect = new_rect(new_pos(0, 0), new_size(320, 240));
int current_frame = 5; // we can update the frame that will be used on all draw calls
sprite_batch_draw(batch, current_frame, screen_rect);
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

// Render the map (software renderer)
tiled_render(disp, tile_test, screen_rect);

// Render the map (hardware renderer)
tiled_render_rdp(tile_test, screen_rect);
```

> tiled_cached.h | tiled_cached.c

This variant can perform really well even if you have more than a few different textures, due to its caching, that tries to prevent texture swaps as much as it can.

It will consume more memory, and will take more time to init, and also doesn't have culling implemented.

```c
// load the map
Size grid_size = new_size(50, 50); // map tiles: 50x50
Size tile_size = new_size_same(16); // tile size: 16x16
TiledCached *tile_test = tiled_cached_init(&memory_pool, tile_sprite, "/path/to/map.map", grid_size, tile_size);

// Render the map
tiled_cached_render(tile_test, screen_rect);
```

## Development

Make sure to use the included clang-format before pushing your code.

**Pull Requests are welcome!**
