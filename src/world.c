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

chunk_t *world_get_chunk(world_t *world, int cx, int cy) {
	if (cx < 0 || cy < 0)
		return NULL;
	if (cx < world->width && cy < world->height) {
		if (world->chunks[cy][cx] == NULL)
			world->chunks[cy][cx] = game_load_chunk(world, cx, cy);
		return world->chunks[cy][cx];
	}
	if (cy >= world->height) {
		world->chunks = realloc(world->chunks, sizeof(chunk_t **) * (cy + 1))	;
		for (int i = world->height; i < cy; i++)
			world->chunks[cy] = calloc(sizeof(chunk_t *), world->width);
		world->height = cy + 1;
	}
	if (cx >= world->width) {
		for (int i = 0; i < world->height; i++) {
			world->chunks[cy] = realloc(world->chunks[cy][i], sizeof(chunk_t *) * (cx + 1));
			for (int k = world->width; k < cx; k++)
				world->chunks[cy][i] = NULL;
		}
		world->width = cx + 1;
	}
	return world_get_chunk(world, cx, cy);
}