#include "game.h"


static void update_screen() {
	int display_screen_width = 0;
	int display_screen_height = 0;
	SDL_GetWindowSize(window, &display_screen_width, &display_screen_height);

	double scale_factor_width = display_screen_width / (double)GAME_WIDTH;
	double scale_factor_height = display_screen_height / (double)GAME_HEIGHT;

	double scale_factor = scale_factor_height;
	if (scale_factor > scale_factor_width)
		scale_factor = scale_factor_height;

	int adjusted_width = (int)(GAME_WIDTH * scale_factor);
    int adjusted_height = (int)(GAME_HEIGHT * scale_factor);

	int x_offset = (display_screen_width - adjusted_width) / 2;
    int y_offset = (display_screen_height - adjusted_height) / 2;

	SDL_Surface *display_screen = SDL_GetWindowSurface(window);
	SDL_FillRect(display_screen, NULL, SDL_MapRGB(display_screen->format, 0, 0, 0));

	SDL_Surface *scaled = SDL_CreateRGBSurfaceWithFormat(0, adjusted_width, adjusted_height, 32, display_screen->format->format);

    SDL_BlitScaled(game_surface, NULL, scaled, NULL);

    SDL_Rect dest_rect = { x_offset, y_offset, adjusted_width, adjusted_height };
    SDL_BlitSurface(scaled, NULL, display_screen, &dest_rect);

    SDL_FreeSurface(scaled);

    SDL_UpdateWindowSurface(window);
}

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
				continue;
			printf("on passe %p at %d %d\n", game_ctx->world->chunks[x][y], i+1, j+1);
			chunks[i + 1][j + 1] = game_ctx->world->chunks[x][y];
		}
	}
	printf("START\n");
	for (int y = 0; y < 3 * CHUNK_SIZE; y++) {
		for (int x = 0; x < 3 * CHUNK_SIZE; x++) {
			chunk_t *chunk = chunks[x / CHUNK_SIZE][y / CHUNK_SIZE];
			printf("chunk (%d %d) %p %d %d\n", x / CHUNK_SIZE, y / CHUNK_SIZE, chunk, x, y);
			if (chunk == NULL)
				continue;
			obj_t obj = chunk->objs[x % CHUNK_SIZE][y % CHUNK_SIZE][0];
			printf("ici !\n");
			if (obj.id == -1 || obj.id >= obj_registry_len || obj_registry[obj.id].frame_len == 0)
				continue;
			printf("ici ?\n");
			int real_x = (x * CHUNK_SIZE) * 60;
			int real_y = (y * CHUNK_SIZE) * 60;
			int frame_id = obj_registry[obj.id].frame_ids[obj.frame_idx];
			SDL_Surface *texture = texture_registry[frame_id].surface;
			printf("texture %p\n", texture);
			if (texture == NULL)
				continue;
			SDL_BlitSurface(texture, NULL, game_surface, &(SDL_Rect){real_x, real_y, texture->w, texture->h});
		}
	}
	update_screen();
}