#include "game.h"


void game_exit(int exit_code) {
	free(game_error_str);
	sprite_free();
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
	free(game_ctx);
	exit(exit_code);
}