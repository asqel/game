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

}

void objects_free() {

}

void game_exit(int exit_code) {
	sdl_exit();
	TTF_Quit();
	IMG_Quit();
	free(game_error_str);
	textures_free();
	objects_free();
	exit(exit_code);
}