#include "game.h"
#include <unistd.h>

int main(int argc, char **argv) {
	game_init(argc, argv);
	game_ctx->world->chunks[0][0]->objs[19][19][1] = game_get_obj(game_get_obj_id("tree"));
	while (1) {

		uint32_t start_time = SDL_GetTicks();
		SDL_Event e;
		for (int i = 1; i < GAME_ACT_ENUM_MAX; i++)
			if (game_ctx->actions[i])
				game_ctx->actions[i]++;
		game_tick();
		game_render();

		uint32_t end_time = SDL_GetTicks();
		uint32_t frame_time = end_time - start_time;
		if (frame_time < 1000 / GAME_FPS) {
			SDL_Delay(1000 / GAME_FPS - frame_time);
		}
		end_time = SDL_GetTicks();
		frame_time = end_time - start_time;
		game_render_strf(10, 10, 255, 255, 255, "FPS: %d", 1000 / frame_time);
		update_screen();
	}
	return 0;
}
