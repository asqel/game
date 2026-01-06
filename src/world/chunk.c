#include "game.h"

chunk_t *game_load_chunk(world_t *world, int x, int y) {
	(void)x;
	(void)y;
	(void)world;
	chunk_t *res = malloc(sizeof(chunk_t));
	for (int i = 0; i < 20; i++) {
		for (int k = 0; k < 20; k++) {
			res->objs[i][k][0].id = 0;
			res->objs[i][k][1].id = 0;
			res->objs[i][k][2].id = 0;
			res->hitbox[i][k] = 0;	
		}
	}
	return res;
}
