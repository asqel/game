#include "game.h"

void update_screen() {
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

void game_render_text(char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b) {
	SDL_Surface *text_surface = TTF_RenderUTF8_Solid(game_ctx->fonts[0], text, (SDL_Color){r, g, b, 0xff});
	SDL_BlitSurface(text_surface, NULL, game_surface, &(SDL_Rect){x, y, text_surface->w, text_surface->h});
	SDL_FreeSurface(text_surface);
}

void game_render_strf(int x, int y, uint8_t r, uint8_t g, uint8_t b, char *text, ...) {
	va_list args;
	va_list args_copy;
	va_start(args, text);
	va_copy(args_copy, args);
	int size = vsnprintf(NULL, 0, text, args_copy);
	va_end(args_copy);
	char *buffer = malloc(size + 1);
	vsprintf(buffer, text, args);
	va_end(args);
	game_render_text(buffer, x, y, r, g, b);
	free(buffer);
}

static void render_border() {
	int cx_real = ((int)game_ctx->player->x / CHUNK_SIZE) * CHUNK_SIZE;
	int cy_real = ((int)game_ctx->player->y / CHUNK_SIZE) * CHUNK_SIZE;

	int offset_x = cx_real - game_ctx->player->x;
	int offset_y = cy_real - game_ctx->player->y;

	int screen_x = GAME_WIDTH / 2 + offset_x * TILE_SIZE;
	int screen_y = GAME_HEIGHT / 2 + offset_y * TILE_SIZE;

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	int color = 0x0000FF;
	SDL_FillRect(game_surface, &(SDL_Rect){screen_x, 0, 1, GAME_HEIGHT}, color);
	SDL_FillRect(game_surface, &(SDL_Rect){screen_x + CHUNK_SIZE * TILE_SIZE, 0, 1, GAME_HEIGHT}, color);
	SDL_FillRect(game_surface, &(SDL_Rect){0, screen_y, GAME_WIDTH, 1}, color);
	SDL_FillRect(game_surface, &(SDL_Rect){0, screen_y + CHUNK_SIZE * TILE_SIZE, GAME_WIDTH, 1}, color);
}

static void render_editor() {
	static int tick_count = 0;
	int texture_id = 0;

	if (tick_count < 15)
		texture_id = game_texture_get_id("editor_0");
	else
		texture_id = game_texture_get_id("editor_1");

	SDL_Surface *texture = texture_registry[texture_id].surface;
	SDL_BlitSurface(texture, NULL, game_surface, &(SDL_Rect){GAME_WIDTH / 2, GAME_HEIGHT / 2, texture->w, texture->h});

	tick_count++;
	tick_count %= 30;

	game_render_strf(0, GAME_HEIGHT - 20, 255, 255, 255, "layer %d", editor_obj_layer);
	game_render_strf(0, GAME_HEIGHT - 40, 255, 255, 255, "obj %d %s", editor_obj_id, obj_registry[editor_obj_id].name);
	render_border();
}

void game_render(uint32_t fps) {
	SDL_FillRect(game_surface, &(SDL_Rect){0, 0, GAME_WIDTH, GAME_HEIGHT}, 0);
	int player_chunk_x = ((int)game_ctx->player->x) / CHUNK_SIZE;
	int player_chunk_y = ((int)game_ctx->player->y) / CHUNK_SIZE;
	int size = game_ctx->player->render_distance;

	chunk_t ***chunks_ptr = malloc(sizeof(chunk_t **) * size);
	for (int i = 0; i < size; i++) {
		chunks_ptr[i] = malloc(sizeof(chunk_t *) * size);
		memset(chunks_ptr[i], 0, sizeof(chunk_t *) * size);
	}
	for (int i = 0; i < size; i++) {
		int real_y = player_chunk_y - size / 2 + i;
		if (real_y < 0 || real_y >= game_ctx->world->height)
			continue;
		for (int k = 0; k < size; k++) {
			int real_x = k + player_chunk_x - size / 2; 
			if (real_x < 0 || real_x >= game_ctx->world->width)
				continue;
			chunks_ptr[i][k] = game_ctx->world->chunks[real_y][real_x];
		}
	}
	game_render_background(chunks_ptr, size);
	game_render_middle(chunks_ptr, size);
	game_render_foreground(chunks_ptr, size);

	for (int i = 0; i < size; i++)
		free(chunks_ptr[i]);
	free(chunks_ptr);
	if (game_ctx->is_editor)
		render_editor();
	if (game_ctx->player->gui)
		game_render_gui();
	game_render_strf(0, 0, 255, 0, 0, "fps %d", fps);
	update_screen();
}
