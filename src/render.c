#include "game.h"


void game_render() {
	int player_chunk_x = ((int)game_ctx->player->x) / CHUNK_SIZE;
	int player_chunk_y = ((int)game_ctx->player->y) / CHUNK_SIZE;
	chunk_t *chunks[3][3] = {{NULL, NULL, NULL},
	                         {NULL, NULL, NULL},
	                         {NULL, NULL, NULL}};
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			int x = player_chunk_x + i;
			int y = player_chunk_y + j;
			if (x < 0 || y < 0 || x >= game_ctx->world->width || y >= game_ctx->world->height)
				game_ctx->world->chunks[x][y] = game_load_chunk(game_ctx->world, x, y);
			chunks[i + 1][j + 1] = game_ctx->world->chunks[x][y];
		}
	}
	for (int y = 0; y < 3 * CHUNK_SIZE; y++) {
		for (int x = 0; x < 3 * CHUNK_SIZE; x++) {
			chunk_t *chunk = chunks[x / CHUNK_SIZE][y / CHUNK_SIZE];
			if (chunk == NULL)
				continue;
			obj_t obj = chunk->objs[x % CHUNK_SIZE][y % CHUNK_SIZE][0];
			if (obj.id == -1 || obj.id >= obj_registry_len || obj_registry[obj.id].frame_len == 0)
				continue;
			int real_x = (x * CHUNK_SIZE) * 60;
			int real_y = (y * CHUNK_SIZE) * 60;
			int frame_id = obj_registry[obj.id].frame_ids[obj.frame_idx];
			SDL_Surface *texture = texture_registry[frame_id].surface;
			if (texture == NULL)
				continue;
			SDL_BlitSurface(texture, NULL, game_surface, &(SDL_Rect){real_x, real_y, texture->w, texture->h});

		}
	}
}