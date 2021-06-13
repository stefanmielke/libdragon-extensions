#pragma once

#include <libdragon.h>
#include "mem_pool.h"

// Called by scene_manager_tick after fnSMSceneChangeCallback.
typedef void (*fnSMCreateCallback)();
// Called by scene_manager_tick. Should return next screen_id (same if didn't change)
typedef short (*fnSMTickCallback)();
// Called by scene_manager_display.
typedef void (*fnSMDisplayCallback)(display_context_t disp);
// Called by scene_manager_tick before fnSMSceneChangeCallback.
typedef void (*fnSMDestroyCallback)();
// Called by scene_manager_tick a frame after the user calls scene_manager_change_scene.
// Should call scene_manager_set_callbacks to set up the scene.
typedef void (*fnSMSceneChangeCallback)(short from_scene_id, short to_scene_id);

typedef struct {
	fnSMCreateCallback create;
	fnSMTickCallback tick;
	fnSMDisplayCallback display;
	fnSMDestroyCallback destroy;
} SceneCallbacks;

typedef struct {
	short current_scene_id;
	short next_scene_id;
	MemZone *scene_memory_pool;
	fnSMSceneChangeCallback change_scene_callback;
	SceneCallbacks scene_callbacks;
} SceneManager;

// Initializes the Scene Manager
// If global_memory_pool is NULL, will use malloc instead.
// If scene_memory_pool is NULL, it will not free memory on screen changes.
// DO NOT use the same MemZone for global_memory_pool and scene_memory_pool.
SceneManager *scene_manager_init(MemZone *global_memory_pool, MemZone *scene_memory_pool,
								 fnSMSceneChangeCallback change_scene_callback);

// Marks scene to change to the scene_id.
void scene_manager_change_scene(SceneManager *scene_manager, short scene_id);

// Sets all the callbacks for your scene. Should be called when changing scenes on your
// fnSMSceneChangeCallback.
// create_callback and destroy_callback are optional.
void scene_manager_set_callbacks(SceneManager *scene_manager, fnSMCreateCallback create_callback,
								 fnSMTickCallback tick_callback,
								 fnSMDisplayCallback display_callback,
								 fnSMDestroyCallback destroy_callback);

// Ticks the scene manager. Will change the scene if needed.
void scene_manager_tick(SceneManager *scene_manager);

void scene_manager_display(SceneManager *scene_manager, display_context_t disp);
