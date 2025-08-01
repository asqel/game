#include "game.h"
#include <unistd.h>

int main(int argc, char **argv) {
	game_init(argc, argv);
	game_ctx->world->chunks[0][0]->objs[19][19][1] = game_get_obj(game_get_obj_id("tree"));
	while (1) {

		uint32_t start_time = SDL_GetTicks();
		SDL_Event e;
		for (int i = 0; i < GAME_ACT_ENUM_MAX; i++)
			if (game_ctx->actions[i])
				game_ctx->actions[i]++;
		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT)
				game_exit(0);
			if (e.type == SDL_KEYDOWN) {
				if (e.key.keysym.scancode == SDL_SCANCODE_W && !game_ctx->actions[GAME_ACT_UP])
					game_ctx->actions[GAME_ACT_UP] = 1;
				else if (e.key.keysym.scancode == SDL_SCANCODE_S && !game_ctx->actions[GAME_ACT_DOWN])
					game_ctx->actions[GAME_ACT_DOWN] = 1;
				else if (e.key.keysym.scancode == SDL_SCANCODE_A && !game_ctx->actions[GAME_ACT_LEFT])
					game_ctx->actions[GAME_ACT_LEFT] = 1;
				else if (e.key.keysym.scancode == SDL_SCANCODE_D && !game_ctx->actions[GAME_ACT_RIGHT])
					game_ctx->actions[GAME_ACT_RIGHT] = 1;
				else if (e.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
					game_exit(0);
			}
			else if (e.type == SDL_KEYUP) {
				if (e.key.keysym.scancode == SDL_SCANCODE_W)
					game_ctx->actions[GAME_ACT_UP] = 0;
				else if (e.key.keysym.scancode == SDL_SCANCODE_S)
					game_ctx->actions[GAME_ACT_DOWN] = 0;
				else if (e.key.keysym.scancode == SDL_SCANCODE_A )
					game_ctx->actions[GAME_ACT_LEFT] = 0;
				else if (e.key.keysym.scancode == SDL_SCANCODE_D)
					game_ctx->actions[GAME_ACT_RIGHT] = 0;
			}
		}
		if (game_ctx->actions[GAME_ACT_LEFT]) {
			game_ctx->player->x -= 0.07;
			game_ctx->player->dir = 3;
		}
		if (game_ctx->actions[GAME_ACT_RIGHT]) {
			game_ctx->player->x += 0.07;
			game_ctx->player->dir = 1;
		}
		if (game_ctx->actions[GAME_ACT_UP]) {
			game_ctx->player->y -= 0.07;
			game_ctx->player->dir = 0;
		}
		if (game_ctx->actions[GAME_ACT_DOWN]) {
			game_ctx->player->y += 0.07;
			game_ctx->player->dir = 2;
		}

		if (game_ctx->player->x < 0)
			game_ctx->player->x = 0;
		if (game_ctx->player->y < 0)
			game_ctx->player->y = 0;
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
