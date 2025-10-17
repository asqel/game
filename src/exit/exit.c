#include "game.h"


void game_exit(int exit_code) {
	free(game_error_str);
	if (is_ttf_init) {
		TTF_CloseFont(game_ctx->fonts[0]);
		TTF_Quit();
	}
	is_ttf_init = 0;

	for (int i = 0; i < game_ctx->world->height; i++) {
		for (int k = 0; k < game_ctx->world->width; k++) {
			free(game_ctx->world->chunks[i][k]);
		}
	}
	for (int i = 0; i < game_ctx->world->height; i++)
		free(game_ctx->world->chunks[i]);
	free(game_ctx->world->chunks);
	free(game_ctx->world);
	free(game_ctx->player);
	free(game_ctx);
	objects_free();
	sprite_free();
	textures_free();
	if (is_img_init)
		IMG_Quit();
	is_img_init = 0;
	if (is_sdl_init)
		sdl_exit();
	is_sdl_init = 0;
	dialogue_exit();
	exit(exit_code);
}