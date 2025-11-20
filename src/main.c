#include "game.h"
#include <unistd.h>

int main(int argc,char **argv) {
	game_init(argc, argv);
	uint32_t last_fps = 0;
	while (1) {

		uint32_t start_time = SDL_GetTicks();
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
		if (!last_fps)
			game_render_strf(10, 10, 255, 255, 255, "FPSé: %d", -1);
		else
			game_render_strf(10, 10, 255, 255, 255, "FPSé: %d", 1000 / last_fps);
		update_screen();
		end_time = SDL_GetTicks();
		frame_time = end_time - start_time;
		last_fps = frame_time;
	}
	return 0;
}
