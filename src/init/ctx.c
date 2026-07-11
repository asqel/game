#include "game.h"

static void parse_args(int argc, char **argv) {
	int i = 1;
	while (i < argc) {
		if (!strcmp(argv[i], "-d")) {
			i++;
			if (i >= argc || !game_path_is_dir(argv[i])) {
				PRINT_ERR("Error: argument -d require a valid path\n");
				game_exit(1);
			}
			game_dir = argv[i];
			normalize_path(game_dir);
			i++;
		}
		else {
			PRINT_ERR("Error: unkown argument '%s'\n", argv[i]);
			game_exit(1);
		}
	}
}


void init_ctx(int argc, char **argv) {
	parse_args(argc, argv);
	game_ctx->player = malloc(sizeof(player_t));
    game_ctx->player->render_distance = 5;
	game_ctx->player->gui = NULL;
	game_ctx->player->entity_infos = NULL;
}
