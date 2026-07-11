#include "game.h"

int init_player() {
	entity_t *ent = entity_add(0, 20, 20, game_ctx->world);
	game_ctx->player->entity_infos = ent->lua_infos;
	return 0;
}
