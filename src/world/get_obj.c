#include "game.h"

obj_t *world_get_obj_at(world_t *world, int x, int y, int layer) {
	if (layer < 0 || layer >= 3)
		return NULL;
	int cy = y / CHUNK_SIZE;
	int cx = x / CHUNK_SIZE;
	if (x < 0 || y < 0)
		return NULL;
	if (cx >= world->width || cy >= world->height || world->chunks[cy][cx] == NULL)
		return NULL;
	return &world->chunks[cy][cx]->objs[y % CHUNK_SIZE][x % CHUNK_SIZE][layer];
}