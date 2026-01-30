#include "game.h"

obj_t world_get_obj(world_t *world, int x, int y, int layer) {
	obj_t air = (obj_t){0};

	chunk_t *chunk = get_chunk_at(x, y, world);
	if (!chunk)
		return air;
	x = x % CHUNK_SIZE;
	y = y % CHUNK_SIZE;
	if (x < 0)
		x += CHUNK_SIZE;
	if (y < 0)
		y += CHUNK_SIZE;
	return chunk->objs[y][x][layer];
}

void world_set_obj(world_t *world, int x, int y, int layer, obj_t obj) {

	chunk_t *chunk = get_chunk_at(x, y, world);
	if (!chunk)
		chunk = world_new_chunk_at(x, y, world);
	x = x % CHUNK_SIZE;
	y = y % CHUNK_SIZE;
	if (x < 0)
		x += CHUNK_SIZE;
	if (y < 0)
		y += CHUNK_SIZE;
	chunk->objs[y][x][layer] = obj;
}
