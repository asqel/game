#include "game.h"

char *game_get_world_path(char *name) {
	if (strlen(name) > GAME_WORLD_NAME_MAX_LEN)
		return NULL;
	char *world_path = malloc(sizeof(char) * (1 + strlen(game_dir) + 1 + 5 + 1 + strlen(name)));
	sprintf(world_path, "%s/world/%s", game_dir, name);
	return world_path;
}

world_t *game_new_world(char *name) {
	world_t *res = malloc(sizeof(world_t));

	strcpy(res->name, name);
	res->chunks = NULL;
	res->path = game_get_world_path(name);
	res->chunks = NULL;
	res->width = 0;
	res->height = 0;
	// TODO!
}

world_t *game_load_world(char *name) {
	if (strlen(name) > GAME_WORLD_NAME_MAX_LEN)
		return NULL;
	char *world_path = game_get_world_path(name);

	FILE *f = fopen(world_path, "rb");
	if (f == NULL) {
		free(world_path);
		return game_new_world(name);
	}
	// TODO!
}