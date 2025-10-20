#include "game.h"

void game_tick_gui() {
	if (game_ctx->player->gui->update)
		game_ctx->player->gui->update(game_ctx->player->gui);
}

void game_close_gui() {
	if (!game_ctx->player->gui)
		return ;
	if (game_ctx->player->gui->free)
		game_ctx->player->gui->free(game_ctx->player->gui);
	free(game_ctx->player->gui);
	game_ctx->player->gui = NULL;
}

void game_render_gui() {
	if (game_ctx->player->gui->render)
	game_ctx->player->gui->render(game_ctx->player->gui);
}

gui_t *game_open_gui(void *data, void (*update)(gui_t *self), void (*render)(gui_t *self), void (*free)(gui_t *self)) {
	gui_t *gui = malloc(sizeof(gui_t));
	gui->data = data;
	gui->free = free;
	gui->render = render;
	gui->update = update;
	game_close_gui();
	game_ctx->player->gui = gui;
	for (int i = 0; i < GAME_ACT_ENUM_MAX; i++)
		game_ctx->actions[i] = 0;
	return gui;
}