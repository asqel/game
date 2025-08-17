#include "game.h"

void player_interact() {
	int x = game_ctx->player->x;
	int y = game_ctx->player->y;

	if (game_ctx->player->dir == 0)
		y--;
	else if (game_ctx->player->dir == 1)
		x++;
	else if (game_ctx->player->dir == 2)
		y++;
	else if (game_ctx->player->dir == 3)
		x--;
	
	obj_t *obj = world_get_obj_at(game_ctx->world, game_ctx->player->x, game_ctx->player->x, 1);
	printf("INTERACT0 %d\n", obj ? obj->id : -1);
	if (obj && obj->id > 0 && obj->id < obj_registry_len && obj_registry[obj->id].interact) {
		obj_registry[obj->id].interact(game_ctx->player->x, game_ctx->player->x);
		return ;
	}
	obj = world_get_obj_at(game_ctx->world, x, y, 1);
	printf("INTERACT1 %d\n", obj ? obj->id : -1);
	if (obj && obj->id > 0 && obj->id < obj_registry_len && obj_registry[obj->id].interact)
		obj_registry[obj->id].interact(x, y);
}
