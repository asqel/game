#include "game.h"

void game_tick_gui() {
	game_ctx->player->gui->update(game_ctx->player->gui);
}

void game_close_gui() {
	game_ctx->player->gui->free(game_ctx->player->gui);
	free(game_ctx->player->gui);
	game_ctx->player->gui = NULL;
}

void game_render_gui() {
	game_ctx->player->gui->render(game_ctx->player->gui);
}

void game_send_event_gui(game_event_t *ev) {	
	game_ctx->player->gui->handle_event(game_ctx->player->gui, ev);
}

int game_is_gui_open() {
	return game_ctx->player->gui != NULL;
}
