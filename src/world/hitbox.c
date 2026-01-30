#include "game.h"

int world_get_hitbox(world_t *world, int x, int y) {
	chunk_t *chunk = get_chunk_at(x, y, world);
	if (!chunk)
		return 0;
	x = x % CHUNK_SIZE;
	y = y % CHUNK_SIZE;
	if (x < 0)
		x += CHUNK_SIZE;
	if (y < 0)
		y += CHUNK_SIZE;
	return chunk->hitbox[y][x];
}

void world_set_hitbox(world_t *world, int x, int y, int state) {
	chunk_t *chunk = get_chunk_at(x, y, world);
	if (!chunk)
		chunk = world_new_chunk_at(x, y, world);
	x = x % CHUNK_SIZE;
	y = y % CHUNK_SIZE;
	if (x < 0)
		x += CHUNK_SIZE;
	if (y < 0)
		y += CHUNK_SIZE;
	chunk->hitbox[y][x]= state;
}

