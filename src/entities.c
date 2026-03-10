#include "game.h"

static void for_each_chunk(void (*func)(chunk_t *, int, int)) {
	int player_chunk_y = game_ctx->player->y / CHUNK_SIZE;	
	int player_chunk_x = game_ctx->player->x / CHUNK_SIZE;	
	int size = game_ctx->player->render_distance;
	for (int i = 0; i < size; i++) {
		int real_y = player_chunk_y - size / 2 + i;

		while (real_y < 0)
			real_y += game_ctx->world->height;
		while (real_y >= game_ctx->world->height)
			real_y -= game_ctx->world->height;

		for (int k = 0; k < size; k++) {
			int real_x = k + player_chunk_x - size / 2; 

			while (real_x < 0)
				real_x += game_ctx->world->width;
			while (real_x >= game_ctx->world->width)
				real_x -= game_ctx->world->width;

			chunk_t *chunk = get_chunk(real_x, real_y, game_ctx->world);
			func(chunk, real_x, real_y);
		}
	}
}

/*
tick
update (vx, vy)
update (x, y)
update death
*/

static void update_tick(chunk_t *chunk, int cx, int cy) {
	(void)cx;
	(void)cy;
	if (!chunk)
		return ;
	for (int i = 0; i < chunk->entities_len; i++) {
		entity_t *ent = &chunk->entities[i];
		entity_info_t *info = &entities_infos[ent->id];
		if (info->on_tick.is_lua) {
			lua_rawgeti(lua_state, LUA_REGISTRYINDEX, info->on_tick.lua_ref);
			lua_rawgeti(lua_state, LUA_REGISTRYINDEX, ent->lua_ref);
			if (lua_pcall(lua_state, 1, 0, 0) != 0) {
					PRINT_ERR("Error: entity tick: %s\n", lua_tostring(lua_state, -1));
					lua_pop(lua_state, 1);
			}
		}
		else if (info->on_tick.c) {
			void (*func)(entity_t *) = info->on_tick.c;
			func(ent);
		}
	}
}

static void update_velocity(chunk_t *chunk, int cx, int cy) {
	(void)cx;
	(void)cy;
	if (!chunk)
		return ;
	for (int i = 0; i < chunk->entities_len; i++) {
		entity_t *ent = &chunk->entities[i];
		if (ent->vx != 0) {
			ent->x += ent->vx;
			ent->vx *= ent->friction;
			ent->is_moving = 1;
			if (-VELOCITY_EPSILON < ent->vx && ent->vx < VELOCITY_EPSILON)
				ent->vx = 0;
		}
		if (ent->vy != 0) {
			ent->y += ent->vy;
			ent->vy *= ent->friction;
			ent->is_moving = 1;
			if (-VELOCITY_EPSILON < ent->vy && ent->vy < VELOCITY_EPSILON)
				ent->vy = 0;
		}
	}
}

static void update_pos(chunk_t *chunk, int cx, int cy) {
	if (!chunk)
		return ;
	for (int i = 0; i < chunk->entities_len; i++) {
		entity_t *ent = &chunk->entities[i];
		if (!ent->is_moving)
			continue;
		int width_as_blocks = game_ctx->world->width * CHUNK_SIZE;
		int height_as_blocks = game_ctx->world->height * CHUNK_SIZE;
		while (ent->x < 0)
			ent->x += width_as_blocks;
		while (ent->x >= width_as_blocks)
			ent->x -= width_as_blocks;
		while (ent->y < 0)
			ent->y += height_as_blocks;
		while (ent->y >= height_as_blocks)
			ent->y -= height_as_blocks;

		ent->is_moving = 0;
		int ent_cx = ent->x / CHUNK_SIZE;
		int ent_cy = ent->y / CHUNK_SIZE;
		if (ent_cx == cx && ent_cy == cy)
			continue;
		chunk_t *new_chunk = get_chunk(ent_cx, ent_cy, game_ctx->world);
		if (!new_chunk)
			new_chunk = world_new_chunk(ent_cx, ent_cy, game_ctx->world);
		new_chunk->entities = realloc(new_chunk->entities, sizeof(entity_t) * (new_chunk->entities_len + 1));
		new_chunk->entities[new_chunk->entities_len] = *ent;
		ent->lua_infos[0] = ent_cx;
		ent->lua_infos[1] = ent_cx;
		ent->lua_infos[2] = new_chunk->entities_len;
		new_chunk->entities_len++;
		
		chunk->entities[i] = chunk->entities[chunk->entities_len - 1];
		chunk->entities[i].lua_infos[0] = cx;
		chunk->entities[i].lua_infos[1] = cy;
		chunk->entities[i].lua_infos[2] = i;
		chunk->entities_len--;
		if (!chunk->entities_len) {
			free(chunk->entities);
			chunk->entities = NULL;
		}
		else if ((chunk->entities_len % 5) == 0)
			chunk->entities = realloc(chunk->entities, sizeof(entity_t) * (chunk->entities_len));

		i--;
	}
}

static void updata_death(chunk_t *chunk, int cx, int cy) {
	(void)cx;
	(void)cy;
	if (!chunk)
		return ;
	// TODO remove entity.hp == 0
}

void entities_tick() {
	for_each_chunk(update_tick);
	for_each_chunk(update_velocity);
	for_each_chunk(update_pos);
	for_each_chunk(updata_death);
}
