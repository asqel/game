#include "game.h"

void game_tick() {
	update_event();
	player_tick();
	entities_tick();
	if (game_ctx->player->gui)
		game_tick_gui();
}
