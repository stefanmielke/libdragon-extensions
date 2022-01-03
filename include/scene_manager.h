#pragma once

#include <libdragon.h>
#include "mem_pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Called by scene_manager_tick after fnSMSceneChangeCallback.
 */
typedef void (*fnSMCreateCallback)();
/**
 * @brief Called by scene_manager_tick.
 *
 * @return next screen_id (same if didn't change).
 */
typedef short (*fnSMTickCallback)();
/**
 * @brief Called by scene_manager_display.
 */
typedef void (*fnSMDisplayCallback)(display_context_t disp);
/**
 * @brief Called by scene_manager_tick before fnSMSceneChangeCallback.
 */
typedef void (*fnSMDestroyCallback)();
/**
 * @brief Called by scene_manager_tick a frame after the user calls scene_manager_change_scene.
 * Should call scene_manager_set_callbacks to set up the scene.
 *
 * @param[in] from_scene_id
 *            Previous scene id.
 * @param[in] to_scene_id
 *            Next scene id.
 */
typedef void (*fnSMSceneChangeCallback)(short from_scene_id, short to_scene_id);

typedef struct {
	/// @see fnSMCreateCallback
	fnSMCreateCallback create;
	/// @see fnSMTickCallback
	fnSMTickCallback tick;
	/// @see fnSMDisplayCallback
	fnSMDisplayCallback display;
	/// @see fnSMDestroyCallback
	fnSMDestroyCallback destroy;
} SceneCallbacks;

typedef struct {
	/// Current loaded scene
	short current_scene_id;
	/// Next scene. Only != 'current_scene_id' if wants to change the scene.
	short next_scene_id;
	/// Scene memory pool. If !NULL will call 'mem_zone_free_all' when changing scenes.
	MemZone *scene_memory_pool;
	/// Function that will be called when changing the scene. Called by 'scene_manager_tick'.
	fnSMSceneChangeCallback change_scene_callback;
	/// Callbacks used by the scene.
	SceneCallbacks scene_callbacks;
} SceneManager;

/**
 * @brief Initializes the Scene Manager. DO NOT use the same MemZone for global_memory_pool and
 * scene_memory_pool.
 *
 * @param[in] global_memory_pool
 *            The global memory pool. If NULL will use malloc.
 * @param[in] scene_memory_pool
 *            Memory pool used by the objects in a scene.
 *            Will get freed every scene change if not NULL.
 * @param[in] change_scene_callback
 *            Function to call when the scene changes. Should call scene_manager_set_callbacks.
 */
SceneManager *scene_manager_init(MemZone *global_memory_pool, MemZone *scene_memory_pool,
								 fnSMSceneChangeCallback change_scene_callback);

/**
 * @brief Changes the scene to 'scene_id'.
 *
 * @param[in] scene_manager
 *            The Scene Manager.
 * @param[in] scene_id
 *            The scene you want to change it to.
 */
void scene_manager_change_scene(SceneManager *scene_manager, short scene_id);

/**
 * @brief Sets all the callbacks for your scene. Should be called when changing scenes on your
 * fnSMSceneChangeCallback.
 *
 * @param[in] scene_manager
 *            The Scene Manager.
 * @param[in] create_callback
 *            Function called when initializing this scene. Can be NULL.
 * @param[in] tick_callback
 *            Function called when calling 'scene_manager_tick'.
 * @param[in] display_callback
 *            Function called when calling 'scene_manager_display'.
 * @param[in] destroy_callback
 *            Function called when destroying this scene. Can be NULL.
 */
void scene_manager_set_callbacks(SceneManager *scene_manager, fnSMCreateCallback create_callback,
								 fnSMTickCallback tick_callback,
								 fnSMDisplayCallback display_callback,
								 fnSMDestroyCallback destroy_callback);

/**
 * @brief Ticks the scene manager. Will change the scene if needed.
 *
 * @param[in] scene_manager
 *            The Scene Manager.
 */
void scene_manager_tick(SceneManager *scene_manager);

/**
 * @brief Ticks the scene manager. Will change the scene if needed.
 *
 * @param[in] scene_manager
 *            The Scene Manager.
 * @param[in] disp
 *            The main display context.
 */
void scene_manager_display(SceneManager *scene_manager, display_context_t disp);

/**
 * @brief Frees up space allocated on the SceneManager. Should only be called if not using a
 * 'global_memory_pool'.
 *
 * @param scene_manager
 *        SceneManager to destroy.
 */
void scene_manager_destroy(SceneManager *scene_manager);

#ifdef __cplusplus
}
#endif