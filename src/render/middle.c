#include "game.h"

typedef struct {
	SDL_Surface *img;
	int x;
	int y;
	int z;
} image_t;

int count_len(chunk_t ***chunks, int size) {
	
}

void game_render_background(chunk_t ***chunks, int size) {
	int offset_x = game_ctx->player->x;
	int offset_y = game_ctx->player->y;

	offset_x -= offset_x % CHUNK_SIZE;
	offset_y -= offset_y % CHUNK_SIZE;
	offset_x -= (size / 2) * CHUNK_SIZE + game_ctx->player->x;
	offset_y -= (size / 2) * CHUNK_SIZE + game_ctx->player->y;

	int frac_x = -(game_ctx->player->x - (int)game_ctx->player->x) * TILE_SIZE;  
	int frac_y = -(game_ctx->player->y - (int)game_ctx->player->y) * TILE_SIZE; 
	int len = count_len(chunks, size);
	for (int y = 0; y < size * CHUNK_SIZE; y++) {
		for (int x = 0; x < size * CHUNK_SIZE; x++) {
			chunk_t *chunk = chunks[y / CHUNK_SIZE][x / CHUNK_SIZE];
			if (!chunk)
				continue;
			int scr_x = GAME_WIDTH / 2 - TILE_SIZE / 2;
			int scr_y = GAME_HEIGHT / 2 - TILE_SIZE / 2;

			scr_x += (offset_x + x) * TILE_SIZE + frac_x;
			scr_y += (offset_y + y) * TILE_SIZE + frac_y;
			obj_t *obj = &chunk->objs[y % CHUNK_SIZE][x % CHUNK_SIZE][0];
			if (!obj->id)
				continue;
			render_obj(obj, scr_x, scr_y);
		}
	}
	
