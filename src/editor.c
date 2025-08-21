#include "game.h"

int editor_obj_id = 0;
int editor_obj_layer = 0;

static void editor_place() {
	int cx = (int)game_ctx->player->x / CHUNK_SIZE;
	int cy = (int)game_ctx->player->y / CHUNK_SIZE;
	int off_x = (int)game_ctx->player->x % CHUNK_SIZE;
	int off_y = (int)game_ctx->player->y % CHUNK_SIZE;
	chunk_t *chunk = world_get_chunk(game_ctx->world, cx, cy);
	chunk->objs[off_y][off_x][editor_obj_layer] = game_get_obj(editor_obj_id);
}

static void editor_remove() {
	int old_obj_id = editor_obj_id;
	editor_obj_id = 0;
	editor_place();
	editor_obj_id = old_obj_id;
}

static void editor_fill() {
	int cx_real = ((int)game_ctx->player->x / CHUNK_SIZE) * CHUNK_SIZE;
	int cy_real = ((int)game_ctx->player->y / CHUNK_SIZE) * CHUNK_SIZE;
	int	old_player_x = game_ctx->player->x;
	int	old_player_y = game_ctx->player->y;

	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			game_ctx->player->x = cx_real + i;
			game_ctx->player->y = cy_real + k;
			editor_place();
		}
	}

	game_ctx->player->x = old_player_x;
	game_ctx->player->y = old_player_y;
}

static void editor_empty() {
	int old_obj_id = editor_obj_id;
	editor_obj_id = 0;
	editor_fill();
	editor_obj_id = old_obj_id;
}

void game_editor_tick() {
	if (game_action_pressed_hold(GAME_ACT_EDIT_RIGHT, 15))
		editor_obj_id++;
	if (game_action_pressed_hold(GAME_ACT_EDIT_LEFT, 15))
		editor_obj_id--;
	if (game_action_pressed_hold(GAME_ACT_EDIT_UP, 15))
		editor_obj_layer++;
	if (game_action_pressed_hold(GAME_ACT_EDIT_DOWN, 15))
		editor_obj_layer--;
	if (editor_obj_id < 0)
		editor_obj_id = obj_registry_len - 1;
	editor_obj_id %= obj_registry_len;
	if (editor_obj_layer < 0)
		editor_obj_layer = 2;
	editor_obj_layer %= 3;

	if (game_action_pressed_hold(GAME_ACT_EDIT_PLACE, 15))
		editor_place();
	if (game_action_pressed_hold(GAME_ACT_EDIT_REMOVE, 15))
		editor_remove();
	if (game_action_pressed_hold(GAME_ACT_EDIT_FILL, 15))
		editor_fill();
	if (game_action_pressed_hold(GAME_ACT_EDIT_EMPTY, 15))
		editor_empty();
	if (game_ctx->actions[GAME_ACT_EDIT_SAVE] == 1)
		game_world_save(game_ctx->world);
}
