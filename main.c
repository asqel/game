#include "game.h"


int main(int argc, char **argv) {
	game_init(argc, argv);

	while (1) {
		game_render();
	}
	return 0;
}
