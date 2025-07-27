#include "game.h"

chunk_t *game_load_chunk(world_t *world, int x, int y) {
	// TODO
	chunk_t *res = malloc(sizeof(chunk_t));
	obj_t grass = game_get_obj(game_get_obj_id("grass"));
	for (int i = 0; i < 20; i++) {
		for (int k = 0; k < 20; k++) {
			res->objs[i][k][0] = grass;
			res->objs[i][k][1].id = 0;
			res->objs[i][k][2].id = 0;
		}
	}
	return res;
}
