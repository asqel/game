#include "game.h"

static int is_layer_empty(chunk_t *chunk, int layer) {
	if (chunk == NULL)
		return 1;
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			if (chunk->objs[i][k][layer].id != 0)
				return 0;
		}
	}
	return 1;
}

void game_world_save(world_t *world) {
	char *path = game_get_world_path(world->name);
	
	FILE *f = fopen(path, "r");

	fwrite(&world->width, 1, sizeof(uint32_t), f);
	fwrite(&world->height, 1, sizeof(uint32_t), f);
	for (int cy = 0; cy < world->height; cy++) {
		for (int cx = 0; cx < world->width; cx++) {
			chunk_t *chunk = world->chunks[cy][cx];
			int layers_empty[3] = {0};
			layers_empty[0] = is_layer_empty(chunk, 0);
			layers_empty[1] = is_layer_empty(chunk, 1);
			layers_empty[2] = is_layer_empty(chunk, 2);
		}
	}

	fclose(f);
}
