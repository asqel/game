#include "game.h"

void game_register_obj(char *name, int frame_len, int *frame_ids) {
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
	obj_registry[obj_registry_len].frame_len = frame_len;
	obj_registry[obj_registry_len].frame_ids = malloc(sizeof(int) * frame_len);
	memcpy(obj_registry[obj_registry_len].frame_ids, frame_ids, sizeof(int) * frame_len);
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