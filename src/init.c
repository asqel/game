#include "game.h"

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

}

