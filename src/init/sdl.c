#include "game.h"


int init_sdl() {
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("game", SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED, GAME_WIDTH, GAME_HEIGHT, SDL_WINDOW_RESIZABLE);

	if (!window) {
		SDL_Quit();
		return 1;
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		SDL_DestroyWindow(window);
		window = NULL;
		SDL_Quit();
		return 1;
	}

	SDL_RenderSetLogicalSize(renderer, GAME_WIDTH, GAME_HEIGHT);
	is_sdl_init = 1;
	return 0;
}
