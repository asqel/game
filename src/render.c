#include "game.h"


static void update_screen() {
    SDL_UpdateTexture(texture, NULL, game_surface->pixels, game_surface->pitch);

    int window_width, window_height;
    SDL_GetWindowSize(window, &window_width, &window_height);

    double scale_factor_width = (double)window_width / (double)GAME_WIDTH;
    double scale_factor_height = (double)window_height / (double)GAME_HEIGHT;

    double scale_factor = (scale_factor_width < scale_factor_height) ? scale_factor_width : scale_factor_height;

    int adjusted_width = (int)(GAME_WIDTH * scale_factor);
    int adjusted_height = (int)(GAME_HEIGHT * scale_factor);

    int x_offset = (window_width - adjusted_width) / 2;
    int y_offset = (window_height - adjusted_height) / 2;

    SDL_Rect dst_rect = {x_offset, y_offset, adjusted_width, adjusted_height};
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
    SDL_RenderPresent(renderer);
}

void game_render() {
	SDL_FillRect(game_surface, &(SDL_Rect){0, 0, GAME_WIDTH, GAME_HEIGHT}, 0);
	int player_chunk_x = ((int)game_ctx->player->x) / CHUNK_SIZE;
	int player_chunk_y = ((int)game_ctx->player->y) / CHUNK_SIZE;
	chunk_t *chunks[3][3] = {{NULL, NULL, NULL},
	                         {NULL, NULL, NULL},
	                         {NULL, NULL, NULL}};
	int top_left[2] = {(player_chunk_x) * CHUNK_SIZE, (player_chunk_y) * CHUNK_SIZE};
	for (int i = -1; i <= 1; i++) {
		for (int k = -1; k <= 1; k++) {
			int x = player_chunk_x + k;
			int y = player_chunk_y + i;
			if (x < 0 || y < 0 || x >= game_ctx->world->width || y >= game_ctx->world->height)
				continue;
			chunks[i + 1][k + 1] = game_ctx->world->chunks[y][x];
		}
	}
	int offset_x =  GAME_WIDTH / 2 - TILE_SIZE / 2 - game_ctx->player->x * TILE_SIZE + top_left[0] * TILE_SIZE;
	int offset_y =  GAME_HEIGHT / 2 - TILE_SIZE / 2 - game_ctx->player->y * TILE_SIZE + top_left[1] * TILE_SIZE;
	for (int y = 0; y < 3 * CHUNK_SIZE; y++) {
		for (int x = 0; x < 3 * CHUNK_SIZE; x++) {
			chunk_t *chunk = chunks[y / CHUNK_SIZE][x / CHUNK_SIZE];
			if (chunk == NULL)
				continue;
			obj_t *objs =  chunk->objs[y % CHUNK_SIZE][x % CHUNK_SIZE];
			for (int i = 0; i < 2; i++) {
				obj_t obj = objs[i];
				if (obj.id == 0 || obj.id >= obj_registry_len || obj_registry[obj.id].frame_len == 0)
					continue;
				int real_x = (x - CHUNK_SIZE) * TILE_SIZE + offset_x;
				int real_y = (y - CHUNK_SIZE) * TILE_SIZE + offset_y;
				int frame_id = obj_registry[obj.id].frame_ids[obj.frame_idx];
				SDL_Surface *texture = texture_registry[frame_id].surface;
				if (texture == NULL)
					continue;
				SDL_BlitSurface(texture, NULL, game_surface, &(SDL_Rect){real_x, real_y, texture->w, texture->h});
			}
			if (y + top_left[1] - CHUNK_SIZE == (int)game_ctx->player->y + 1) {
				SDL_Surface *texture = texture_registry[game_texture_get_id("mc_d")].surface;
				SDL_BlitSurface(texture, NULL, game_surface, &(SDL_Rect){GAME_WIDTH / 2 -  texture->w / 2, GAME_HEIGHT / 2 -  texture->h / 2 , texture->w, texture->h});
			}
			obj_t obj = objs[2];
			if (obj.id == 0 || obj.id >= obj_registry_len || obj_registry[obj.id].frame_len == 0)
				continue;
			int real_x = (x - CHUNK_SIZE) * TILE_SIZE + offset_x;
			int real_y = (y - CHUNK_SIZE) * TILE_SIZE + offset_y;
			int frame_id = obj_registry[obj.id].frame_ids[obj.frame_idx];
			SDL_Surface *texture = texture_registry[frame_id].surface;
			if (texture == NULL)
				continue;
			SDL_BlitSurface(texture, NULL, game_surface, &(SDL_Rect){real_x, real_y, texture->w, texture->h});
		}
	}
	update_screen();
}