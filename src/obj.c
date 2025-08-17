#include "game.h"

void game_register_obj(char *name, int sprite_id, int hithox, void *interact) {
	if (strlen(name) > OBJ_NAME_LENGTH) {
		PRINT_ERR("Error: Object name too long: '%s'\n", name);
		return;
	}
	void *tmp;
	for (int i = 0; i < obj_registry_len; i++) {
		if (strcmp(obj_registry[i].name, name) == 0) {
			PRINT_ERR("Error: Object already exists: '%s'\n", name);
			return;
		}
	}
	tmp = realloc(obj_registry, sizeof(obj_info_t) * (obj_registry_len + 1));
	if (!tmp) {
		PRINT_ERR("Error: Memory allocation failed (game_register_obj)\n");
		game_exit(1);
	}
	obj_registry = tmp;
	strcpy(obj_registry[obj_registry_len].name, name);
	obj_registry[obj_registry_len].sprite_id = sprite_id;
	obj_registry[obj_registry_len].has_hitbox = hithox;
	obj_registry[obj_registry_len].interact = interact;
	obj_registry_len++;
}

int game_get_obj_id(char *name) {
	for (int i = 0; i < obj_registry_len; i++) {
		if (strcmp(obj_registry[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}

obj_t game_get_obj(int id) {
	if (id <= 0 || id >= obj_registry_len) {
		obj_t empty_obj = {0};
		return empty_obj;
	}
	obj_t obj;
	obj.id = id;
	obj.data = 0;
	obj.sprite.sprite_id = obj_registry[id].sprite_id;
	obj.sprite.frame_idx = 0;
	if (obj.sprite.sprite_id >= 0 && obj.sprite.sprite_id < sprite_registry_len) {
		obj.sprite.current_frame_ttl = sprite_registry[obj.sprite.sprite_id].frame_len_tick;
		obj.sprite.state = sprite_registry[obj.sprite.sprite_id].state;
	}
	return obj;
}