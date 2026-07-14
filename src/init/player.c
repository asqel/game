#include "game.h"

int init_player() {
	entity_t *ent = entity_add(0, 16, 16, game_ctx->world);
	game_ctx->player->entity_infos = ent->lua_infos;
	printf("palyer sprite %u\n", ent->sprite.sprite_id);
	printf("palyer sprite %u\n", ent->sprite.state);
	return 0;
}
