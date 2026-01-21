#include "game.h"

static void parse_args(int argc, char **argv) {
	int i = 1;
	while (i < argc) {
		if (!strcmp(argv[i], "--editor")) {
			i++;
			game_ctx->is_editor = 1;
		}
		else if (!strcmp(argv[i], "-d")) {
			i++;
			if (i >= argc || !game_path_is_dir(argv[i])) {
				PRINT_ERR("Error: argument -d require a valid path\n");
				game_exit(1);
			}
			free(game_dir);
			game_dir = strdup(argv[i]);
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
	for (int i = 0; i < GAME_ACT_ENUM_MAX; i++)
		game_ctx->actions[i] = 0;
	game_ctx->player = calloc(1, sizeof(player_t));
	game_ctx->world = game_load_world("start");
    game_ctx->player->x = 18;
    game_ctx->player->y = 18;
    game_ctx->player->render_distance = 5;

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
}
