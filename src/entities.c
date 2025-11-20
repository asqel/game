#include "game.h"

void tick_entity(entity_t *ent) {
	entity_info_t *info = get_entity_info(ent->id);
	if (!info)
		return ;
	if (info->on_tick.is_lua) {
		// TODO
	}
	else if (info->on_tick.c) {
		ent->has_to_die = ((int (*)(entity_t *self))info->on_tick.c)(ent);
	}
}

void entities_tick() {
	int player_chunk_y = game_ctx->player->y / CHUNK_SIZE;	
	int player_chunk_x = game_ctx->player->x / CHUNK_SIZE;	
	int size = game_ctx->player->render_distance;
	for (int i = 0; i < size; i++) {
		int real_y = player_chunk_y - size / 2 + i;
		if (real_y < 0 || real_y >= game_ctx->world->height)
			continue;
		for (int k = 0; k < size; k++) {
			int real_x = k + player_chunk_x - size / 2; 
			if (real_x < 0 || real_x >= game_ctx->world->width)
				continue;
			if (!game_ctx->world->chunks[real_y][real_x])
				continue;
			entity_t *entities = game_ctx->world->chunks[real_y][real_x]->entities;
			int entities_len = game_ctx->world->chunks[real_y][real_x]->entities_len;
			for (int l = 0 ; l < entities_len; l++)
				tick_entity(&entities[l]);
		}
	}
}
