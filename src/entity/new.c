#include "game.h"

entity_t *entity_add(uint32_t id, double x, double y, world_t *world) {
	int width = world->width * CHUNK_SIZE;
	int height = world->height * CHUNK_SIZE;
	while (x < 0)
		x += width;
	while (x >= width)
		x -= width;
	while (y < 0)
		y += height;
	while (y >=	height)
		y -= height;
	
	int cx = (int)x / CHUNK_SIZE;
	int cy = (int)y / CHUNK_SIZE;
	chunk_t *chunk = get_chunk(cx, cy, world);
	if (!chunk)
		chunk = world_new_chunk(cx, cy, world);
	chunk->entities = realloc(chunk->entities, sizeof(entity_t) * (chunk->entities_len + 1));
	entity_t *ent = &chunk->entities[chunk->entities_len++];
	if (id >= (uint32_t)entities_infos_len)
		id = 0;
	ent->x = x;
	ent->y = y;
	ent->vx = 0;
	ent->vy = 0;
	ent->id = id;
	ent->sprite = get_sprite_by_id(entities_infos[id].sprite_id);
	ent->hp = entities_infos[id].hp;
	ent->world_hitbox_x = entities_infos[id].default_world_hitbox_x;
	ent->world_hitbox_y = entities_infos[id].default_world_hitbox_y;
	ent->world_hitbox_w = entities_infos[id].default_world_hitbox_w;
	ent->world_hitbox_h = entities_infos[id].default_world_hitbox_h;
	ent->lua_infos = game_new_lua_udata("entity_t", sizeof(int) * 3, &ent->lua_ref);
	ent->lua_infos[0] = cx;
	ent->lua_infos[1] = cy;
	ent->lua_infos[2] = chunk->entities_len - 1;
	ent->drag = entities_infos[id].drag;
	ent->is_moving = 0;
	ent->data_ref = LUA_REFNIL;
	ent->sprite_depth = entities_infos[id].sprite_depth;
	return ent;
}

void entity_destroy(entity_t *ent) {
	luaL_unref(lua_state, LUA_REGISTRYINDEX, ent->lua_ref);
	ent->lua_ref = LUA_REFNIL;
	ent->lua_infos[0] = -1;
	ent->lua_infos[1] = -1;
	ent->lua_infos[2] = -1;
	ent->lua_infos = NULL;
	luaL_unref(lua_state, LUA_REGISTRYINDEX, ent->data_ref);
	ent->data_ref = LUA_REFNIL;
}
