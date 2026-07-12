#include "game.h"

noreturn void game_exit(int exit_code) {
	for (int i = 0; i < game_ctx->world->height; i++) {
		for (int k = 0; k < game_ctx->world->width; k++) {
			chunk_t *chunk = game_ctx->world->chunks[i][k];
			if (!chunk)
				continue;

			for (int p = 0; p < chunk->entities_len; p++)
				entity_destroy(&chunk->entities[p]);
			free(chunk->entities);
			chunk->entities = NULL;
			chunk->entities_len = 0;
			free(chunk);
		}
	}
	for (int i = 0; i < game_ctx->world->height; i++)
		free(game_ctx->world->chunks[i]);
	free(game_ctx->world->chunks);
	free(game_ctx->world);
	free(game_ctx->player);
	objects_free();
	sprite_free();
	textures_free();
	if (is_img_init)
		IMG_Quit();
	is_img_init = 0;
	if (is_sdl_init)
		sdl_exit();
	is_sdl_init = 0;
	game_free_dialogues();
	free(game_ctx);
	lua_close(lua_state);
	lua_state = NULL;
	exit(exit_code);
}