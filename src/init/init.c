#include "game.h"
// init order
/*
	sdl
	img
	textures
	dialogue
	ctx
	lua
	player
	lua script

*/

void game_init(int argc, char **argv) {
	game_ctx = malloc(sizeof(game_t));
	if (game_ctx == NULL) {
		PRINT_ERR("Error: Failed to allocate game context\n");
		game_exit(1);
	}
	*game_ctx = (game_t){0};
	game_dir = "./";
	if (init_sdl())
		exit(1);
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		PRINT_ERR("Error: Failed to initialize SDL Image: %s\n", IMG_GetError());
		game_exit(1);
	}
	is_img_init = 1;
	init_textures();
	init_sprite();
	init_objects();
	if (dialogue_init())
		game_exit(1);
	init_ctx(argc, argv);
	entity_register("", 0, LUA_REFNIL, 100, (double []){0, 0, 1, 1}, DEFAULT_FRICTION, 0);
	if (init_lua())
		game_exit(1);
	if (init_lua_script())
		game_exit(1);
	game_ctx->world = game_load_world("start");
	if (init_player())
		game_exit(1);
}

