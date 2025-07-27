#include "game.h"

int init_sdl() {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);

	window = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_RESIZABLE);

	if (!window) {
		SDL_Quit();
		return 1;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_Quit();
		return 1;
	}

	game_surface = SDL_CreateRGBSurfaceWithFormat(0, GAME_WIDTH, GAME_HEIGHT, 32,
		SDL_PIXELFORMAT_RGBA32);
	if (game_surface == NULL) {
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		window = NULL;
		renderer = NULL;
		SDL_Quit();
		return 1;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
		SDL_TEXTUREACCESS_STREAMING, GAME_WIDTH, GAME_HEIGHT);
	return 0;
}

void init_texture_callback(const char *filepath) {
	if (strlen(filepath) < 4 || strcmp(filepath + strlen(filepath) - 3, ".tx") != 0)
		return;
	int path_len = strlen(filepath);
	char *png_path = malloc(path_len + 2);
	if (!png_path) {
		PRINT_ERR("Error: Memory allocation failed (init_texture_callback)\n");
		return;
	}
	strcpy(png_path, filepath);
	png_path[path_len - 2] = 'p';
	png_path[path_len - 1] = 'n';
	png_path[path_len] = 'g';
	png_path[path_len + 1] = '\0';
	if (!game_path_is_file(png_path)) {
		free(png_path);
		return;
	}
	game_load_tx(png_path, filepath);
}

int init_textures() {
	char *assets_path = malloc(strlen(game_dir) + 10);
	if (!assets_path) {
		PRINT_ERR("Error: Memory allocation failed (init_textures)\n");
		return 1;
	}
	sprintf(assets_path, "%s/assets", game_dir);

	game_list_files(assets_path, init_texture_callback);
	printf("texture number %d\n", texture_registry_len);
	free(assets_path);
	return 0;
}

int init_sprites() {
	register_sprite((int []){game_texture_get_id("grass0")}, 1, 0, "grass", 0);
	register_sprite((int []){game_texture_get_id("tree"), game_texture_get_id("mc_d0")}, 2, 15, "tree", SPRITE_MASK_LOOP);
	return 0;
}

int init_objects() {
	game_register_obj("air", 0);
	game_register_obj("grass", get_sprite_id("grass"));
	game_register_obj("tree", get_sprite_id("tree"));
	return 0;
}

void game_init(int argc, char **argv) {
	if (argc > 2) {
		PRINT_ERR("Error: %s takes at most 1 argument\n", argv[0]);
		exit(1);
	}
	if (argc == 2)
		game_dir = argv[1];
	if (!game_path_is_dir(game_dir)) {
		PRINT_ERR("Error: %s is not a valid directory\n", game_dir);
		exit(1);
	}

	if (init_sdl()) {
		PRINT_ERR("Error: Failed to initialize SDL\n");
		exit(1);
	}
	if (TTF_Init() == -1) {
		PRINT_ERR("Error: Failed to initialize SDL TTF: %s\n", TTF_GetError());
		sdl_exit();
		exit(1);
	}
	if (IMG_Init(IMG_INIT_PNG) == 0) {
		PRINT_ERR("Error: Failed to initialize SDL IMG: %s\n", IMG_GetError());
		TTF_Quit();
		sdl_exit();
		exit(1);
	}

	if (init_textures()) {
		PRINT_ERR("Error: Failed to initialize textures\n\t%s\n", game_get_error());
		game_exit(1);
	}
	if (init_sprites()) {
		PRINT_ERR("Error: Failed to initialize sprites\n\t%s\n", game_get_error());
		game_exit(1);
	}
	if (init_objects()) {
		PRINT_ERR("Error: Failed to initialize objects\n\t%s\n", game_get_error());
		game_exit(1);
	}
	game_ctx = malloc(sizeof(game_t));
	if (game_ctx == NULL) {
		PRINT_ERR("Error: Failed to allocate game context\n");
		game_exit(1);
	}
	*game_ctx = (game_t){0};
	memset(game_ctx->actions, 0, sizeof(uint32_t) * GAME_ACT_ENUM_MAX);
	game_ctx->player = malloc(sizeof(player_t));
	game_ctx->world = game_load_world("start");
	game_ctx->player->x = 0;
	game_ctx->player->y = 0;
	game_ctx->world->chunks = calloc(sizeof(chunk_t **), 2);
	game_ctx->world->chunks[0] = calloc(sizeof(chunk_t *), 2);
	game_ctx->world->chunks[1] = calloc(sizeof(chunk_t *), 2);
	game_ctx->world->chunks[0][0] = game_load_chunk(game_ctx->world, 0, 0);
	game_ctx->world->chunks[1][1] = game_load_chunk(game_ctx->world, 0, 0);
	game_ctx->world->height = 2;
	game_ctx->world->width = 2;

	for (int i = 0; i < 4; i++) {
		for (int k = 0; k < 3; k++) {
			char name[] = "mc_d0";
			name[3] = "urdl"[i];
			name[4] = k + '0';
			int texture_id = game_texture_get_id(name);
			game_ctx->player_textures[i][k] = texture_registry[texture_id].surface;
		}
	}
	game_ctx->player->dir = 2;

	char *font_path = malloc(sizeof(char) * (strlen(game_dir) + strlen("/assets/PressStart2P-Regular.ttf") + 1));
	sprintf(font_path, "%s/assets/PressStart2P-Regular.ttf", game_dir);
	game_ctx->fonts[0] = TTF_OpenFont(font_path, 16);
	free(font_path);
}

