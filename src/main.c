#include "game.h"
#include <unistd.h>

int main(int argc,char **argv) {
	game_init(argc, argv);
	while (1) {

		game_loop_start();

		for (int i = 1; i < GAME_ACT_ENUM_MAX; i++)
			if (game_ctx->actions[i])
				game_ctx->actions[i]++;

		game_tick();
		game_render(game_get_fps());

		game_loop_end();

	}
	return 0;
}
