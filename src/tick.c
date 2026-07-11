#include "game.h"

void game_tick() {
	update_event();
	entities_tick();
	if (game_ctx->player->gui)
		game_tick_gui();
}
