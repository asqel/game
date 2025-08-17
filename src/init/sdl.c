#include "game.h"


int init_sdl() {
	SDL_Init(SDL_INIT_VIDEO);
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
	is_sdl_init = 1;
	return 0;
}
