
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