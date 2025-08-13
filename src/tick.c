#include "game.h"

void game_tick() {
	update_event();
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
}
