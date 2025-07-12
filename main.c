#include "game.h"


int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_VIDEO);
	TTF_Init();
	IMG_Init(IMG_INIT_PNG);
	game_init(argc, argv);
	return 0;	
}
