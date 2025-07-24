#include "game.h"

chunk_t *game_load_chunk(world_t *world, int x, int y) {
	// TODO
	chunk_t *res = malloc(sizeof(chunk_t));
	int grass_id = game_get_obj_id("grass");
	int tall_grass_id = game_get_obj_id("tall_grass");
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 20; j++) {
			res->objs[i][j][0].id = grass_id;

			res->objs[i][j][0].data = 0;
			res->objs[i][j][0].frame_idx = 0;
			res->objs[i][j][0].next_frame_time = 0;
			for (int k = 1; k < 3; k++) {
				res->objs[i][j][k] = (obj_t){0};
			}
		}
	}
	return res;
}
