#include "game.h"

void update_screen() {
	//SDL_UpdateTexture(texture, NULL, game_surface->pixels, game_surface->pitch);

	//int window_width, window_height;
	//SDL_GetWindowSize(window, &window_width, &window_height);

	//double scale_factor_width = (double)window_width / (double)GAME_WIDTH;
	//double scale_factor_height = (double)window_height / (double)GAME_HEIGHT;

	//double scale_factor = (scale_factor_width < scale_factor_height) ? scale_factor_width : scale_factor_height;

	//int adjusted_width = (int)(GAME_WIDTH * scale_factor);
	//int adjusted_height = (int)(GAME_HEIGHT * scale_factor);

	//int x_offset = (window_width - adjusted_width) / 2;
	//int y_offset = (window_height - adjusted_height) / 2;

	//SDL_Rect dst_rect = {x_offset, y_offset, adjusted_width, adjusted_height};
	//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	//SDL_RenderClear(renderer);

	//SDL_RenderCopy(renderer, texture, NULL, &dst_rect);
	SDL_RenderPresent(renderer);
}

void game_render() {
	if (!game_ctx->world)
		return ;
	//SDL_FillRect(game_surface, &(SDL_Rect){0, 0, GAME_WIDTH, GAME_HEIGHT}, 0);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // noir
SDL_RenderClear(renderer);
	int player_chunk_x = game_ctx->player->entity_infos[0];
	int player_chunk_y = game_ctx->player->entity_infos[1];
	int player_idx = game_ctx->player->entity_infos[2];
	entity_t *player = &game_ctx->world->chunks[player_chunk_y][player_chunk_x]->entities[player_idx];
	player_chunk_x = player->x / CHUNK_SIZE;
	player_chunk_y = player->y / CHUNK_SIZE;
	
	int size = game_ctx->player->render_distance;

	chunk_t ***chunks_ptr = malloc(sizeof(chunk_t **) * size);
	for (int i = 0; i < size; i++) {
		chunks_ptr[i] = malloc(sizeof(chunk_t *) * size);
		memset(chunks_ptr[i], 0, sizeof(chunk_t *) * size);
	}
	for (int i = 0; i < size; i++) {
		int real_y = player_chunk_y - size / 2 + i;

		while (real_y < 0)
			real_y += game_ctx->world->height;
		while (real_y >= game_ctx->world->height)
			real_y -= game_ctx->world->height;

		for (int k = 0; k < size; k++) {
			int real_x = k + player_chunk_x - size / 2; 

			while (real_x < 0)
				real_x += game_ctx->world->width;
			while (real_x >= game_ctx->world->width)
				real_x -= game_ctx->world->width;

			chunks_ptr[i][k] = get_chunk(real_x, real_y, game_ctx->world);
			if (chunks_ptr[i][k]) {
				chunks_ptr[i][k]->rx = real_x;
				chunks_ptr[i][k]->ry = real_y;
			}
		}
	}
	game_render_background(chunks_ptr, size, player->x, player->y);
	game_render_middle(chunks_ptr, size, player->x, player->y);
	game_render_foreground(chunks_ptr, size, player->x, player->y);

	for (int i = 0; i < size; i++)
		free(chunks_ptr[i]);
	free(chunks_ptr);
	if (game_ctx->player->gui)
		game_render_gui();
}
