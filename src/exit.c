#include "game.h"

void sdl_exit() {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
	if (game_surface) {
		SDL_FreeSurface(game_surface);
		game_surface = NULL;
	}
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
	SDL_Quit();
}

void textures_free() {
	for (int i = 0; i < texture_registry_len; i++)
		SDL_FreeSurface(texture_registry[i].surface);
	free(texture_registry);
}

void objects_free() {
	for (int i = 0; i < obj_registry_len; i++)
		free(obj_registry[i].frame_ids);
	free(obj_registry);
}

void game_exit(int exit_code) {
	free(game_error_str);
	textures_free();
	objects_free();
	IMG_Quit();
	TTF_CloseFont(game_ctx->fonts[0]);
	TTF_Quit();
	sdl_exit();
	for (int i = 0; i < game_ctx->world->height; i++) {
		for (int k = 0; k < game_ctx->world->width; k++) {
			free(game_ctx->world->chunks[i][k]);
		}
	}
	for (int i = 0; i < game_ctx->world->height; i++)
		free(game_ctx->world->chunks[i]);
	free(game_ctx->world->chunks);
	free(game_ctx->world->path);
	free(game_ctx->world);
	free(game_ctx->player);
	exit(exit_code);
}