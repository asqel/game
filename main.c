#include "game.h"
#include <unistd.h>

int main(int argc, char **argv) {
	game_init(argc, argv);

	while (1) {

		game_render();
		usleep(1000 * 1000 * 2);
		exit(1);
	}
	return 0;
}
