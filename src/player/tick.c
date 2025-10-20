#include "game.h"

void player_tick() {
	if (game_ctx->player->gui)
		return ;
	if (game_ctx->actions[GAME_ACT_LEFT]) {
    	game_ctx->player->dir = 3;
		player_move(-0.07, 0);
    }
    if (game_ctx->actions[GAME_ACT_RIGHT]) {
		player_move(0.07, 0);
    	game_ctx->player->dir = 1;
    }
    if (game_ctx->actions[GAME_ACT_UP]) {
		player_move(0, -0.07);
    	game_ctx->player->dir = 0;
    }
    if (game_ctx->actions[GAME_ACT_DOWN]) {
		player_move(0, 0.07);
    	game_ctx->player->dir = 2;
    }
	if (game_action_pressed_hold(GAME_ACT_INTERACT, 15) && !game_ctx->is_editor)
		player_interact();
	if (game_ctx->player->x < 0)
		game_ctx->player->x = 0;
	if (game_ctx->player->y < 0)
		game_ctx->player->y = 0;
	if (game_ctx->is_editor)
		game_editor_tick();

}
