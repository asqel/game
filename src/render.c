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

static void game_render_player() {
	if (game_ctx->is_editor)
		return ;
	SDL_Surface *texture = NULL;
	int action_duration = (game_ctx->actions[GAME_ACT_UP + game_ctx->player->dir] * 100 / GAME_FPS) % 100;
	if (game_ctx->actions[GAME_ACT_UP + game_ctx->player->dir] == 0)
		action_duration = 0;
	else {
    	action_duration = ((action_duration / 17) % 2) + 1;
	}
	texture = game_ctx->player_textures[game_ctx->player->dir][action_duration];
	SDL_BlitSurface(texture, NULL, game_surface, &(SDL_Rect){GAME_WIDTH / 2 -  texture->w / 2, GAME_HEIGHT / 2 -  texture->h / 2 , texture->w, texture->h});
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

static void game_render_layer(int layer, chunk_t *chunks[3][3], int offset_x, int offset_y, int render_player) {

	int line_y_bottom = (0 - CHUNK_SIZE) * TILE_SIZE + offset_y + TILE_SIZE;
	for (int y = 0; y < 3 * CHUNK_SIZE; y++) {
		if (render_player && line_y_bottom > GAME_HEIGHT / 2 + TILE_SIZE / 2 && line_y_bottom - TILE_SIZE <= GAME_HEIGHT / 2 + TILE_SIZE / 2)
			game_render_player();
		for (int x = 0; x < 3 * CHUNK_SIZE; x++) {
			chunk_t *chunk = chunks[y / CHUNK_SIZE][x / CHUNK_SIZE];
			if (chunk == NULL)
				continue;
			obj_t *obj =  &chunk->objs[y % CHUNK_SIZE][x % CHUNK_SIZE][layer];
			SDL_Surface *texture = NULL;
			if (obj->id != 0) {
				texture = game_get_sprite_texture(&obj->sprite);
				game_sprite_tick(&obj->sprite);
			}

			if (texture != NULL) {
				int real_x = (x - CHUNK_SIZE) * TILE_SIZE + offset_x - texture->w / 2 + TILE_SIZE / 2;
				int real_y = line_y_bottom - texture->h;
				SDL_BlitSurface(texture, NULL, game_surface, &(SDL_Rect){real_x, real_y, texture->w, texture->h});
			}
		}
		line_y_bottom += TILE_SIZE;
	}
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
			if (x >= 0 && y >= 0 && x < game_ctx->world->width && y < game_ctx->world->height)
				chunks[i + 1][k + 1] = game_ctx->world->chunks[y][x];
		}
	}
	int offset_x =  GAME_WIDTH / 2 - TILE_SIZE / 2 - game_ctx->player->x * TILE_SIZE + top_left[0] * TILE_SIZE + TILE_SIZE / 2;
	int offset_y =  GAME_HEIGHT / 2 - TILE_SIZE / 2 - game_ctx->player->y * TILE_SIZE + top_left[1] * TILE_SIZE + TILE_SIZE / 2;
	game_render_layer(0, chunks, offset_x, offset_y, 0);
	game_render_layer(1, chunks, offset_x, offset_y, 1);
	game_render_layer(2, chunks, offset_x, offset_y, 0);
	if (game_ctx->is_editor)
		render_editor();
	if (game_ctx->player->gui)
		game_render_gui();
}