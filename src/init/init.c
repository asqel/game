#include "game.h"
// init order
/*
	sdl
	img
	textures
	sprites
	obj
	ctx
	ttf
*/

void test()  {
	open_dialogue("aa", NULL, 2, 0, 1);
}

void game_init(int argc, char **argv) {
	if (init_sdl())
		exit(1);
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		PRINT_ERR("Error: Failed to initialize SDl Image\n");
		game_exit(1);
	}
	is_img_init = 1;
	if (init_textures())
		game_exit(1);
	if (init_sprites())
		game_exit(1);
	if (TTF_Init() == -1)
		game_exit(1);
	game_ctx = malloc(sizeof(game_t));
	if (game_ctx == NULL) {
		PRINT_ERR("Error: Failed to allocate game context\n");
		game_exit(1);
	}
	*game_ctx = (game_t){0};
	if (init_lua())
		game_exit(1);
	if (dialogue_init())
		game_exit(1);
	init_ctx(argc, argv);
	char *font_path = malloc(sizeof(char) * (strlen(game_dir) + strlen("/assets/PressStart2P-Regular.ttf") + 1));
	sprintf(font_path, "%s/assets/PressStart2P-Regular.ttf", game_dir);
	game_ctx->fonts[0] = TTF_OpenFont(font_path, 16);
	game_ctx->fonts_height[0] = 20;
	free(font_path);
}

