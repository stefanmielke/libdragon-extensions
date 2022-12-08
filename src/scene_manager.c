#include "../include/scene_manager.h"

SceneManager *scene_manager_init(MemZone *global_memory_pool, MemZone *scene_memory_pool,
								 fnSMSceneChangeCallback change_scene_callback) {
	SceneManager *scene_manager = NULL;
	if (global_memory_pool)
		scene_manager = mem_zone_alloc(global_memory_pool, sizeof(SceneManager));
	else
		scene_manager = malloc(sizeof(SceneManager));

	scene_manager->change_scene_callback = change_scene_callback;
	scene_manager->current_scene_id = -1;
	scene_manager->scene_memory_pool = scene_memory_pool;

	return scene_manager;
}

void scene_manager_change_scene(SceneManager *scene_manager, short scene_id) {
	scene_manager->next_scene_id = scene_id;
}

void scene_manager_set_callbacks(SceneManager *scene_manager, fnSMCreateCallback create_callback,
								 fnSMTickCallback tick_callback,
								 fnSMDisplayCallback display_callback,
								 fnSMDestroyCallback destroy_callback) {
	scene_manager->scene_callbacks.create = create_callback;
	scene_manager->scene_callbacks.tick = tick_callback;
	scene_manager->scene_callbacks.display = display_callback;
	scene_manager->scene_callbacks.destroy = destroy_callback;

	if (!scene_manager->scene_callbacks.tick || !scene_manager->scene_callbacks.display)
		abort();
}

void scene_manager_tick(SceneManager *scene_manager) {
	// change scene if needed
	if (scene_manager->current_scene_id != scene_manager->next_scene_id) {
		if (scene_manager->current_scene_id >= 0) {
			if (scene_manager->scene_callbacks.destroy)
				scene_manager->scene_callbacks.destroy();
			if (scene_manager->scene_memory_pool)
				mem_zone_free_all(scene_manager->scene_memory_pool);
		}

		scene_manager->change_scene_callback(scene_manager->current_scene_id,
											 scene_manager->next_scene_id);

		scene_manager->current_scene_id = scene_manager->next_scene_id;

		if (scene_manager->scene_callbacks.create)
			scene_manager->scene_callbacks.create();
	}

	scene_manager->next_scene_id = scene_manager->scene_callbacks.tick();
}

void scene_manager_display(SceneManager *scene_manager, surface_t *disp) {
	scene_manager->scene_callbacks.display(disp);
}

void scene_manager_destroy(SceneManager *scene_manager) {
	free(scene_manager);
}
