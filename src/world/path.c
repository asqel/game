#include "game.h"

char *game_get_world_path(char *name) {
	int len = strlen(name);
	if (len > GAME_WORLD_NAME_MAX_LEN)
		return NULL;
	char *world_path = malloc(sizeof(char) * (1 + strlen(game_dir) + 1 + 5 + 1 + len));
	sprintf(world_path, "%s/world/%s", game_dir, name);
	return world_path;
}

int game_world_exists(char *name) {
	char *path = game_get_world_path(name);
	if (!game_path_is_file(path)) {
		free(path);
		return 0;
	}
	free(path);
	return 1;
}
