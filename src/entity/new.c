#include "game.h"

static void init_metatable() {
	static int is_init = 0;
	if (is_init)
		return ;
	is_init = 1;
	luaL_newmetatable(lua_state, "entity_t");
	lua_pop(lua_state, 1);
}

entity_t *entity_add(uint32_t id, int x, int y, world_t *world) {
	init_metatable();
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
	
	int cx = x / CHUNK_SIZE;
	int cy = y / CHUNK_SIZE;
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
	ent->data.is_lua = 0;
	ent->data.c = NULL;
	ent->id = id;
	ent->sprite = get_sprite_by_id(entities_infos[id].sprite_id);
	ent->hp = entities_infos[id].hp;
	ent->hitbox_x = entities_infos[id].default_hitbox_x;
	ent->hitbox_y = entities_infos[id].default_hitbox_y;
	ent->hitbox_w = entities_infos[id].default_hitbox_w;
	ent->hitbox_h = entities_infos[id].default_hitbox_h;
	ent->lua_infos = lua_newuserdata(lua_state, sizeof(int) * 3);
	luaL_getmetatable(lua_state, "entity_t");
	lua_setmetatable(lua_state, -2);
	ent->lua_ref = luaL_ref(lua_state, LUA_REGISTRYINDEX);
	ent->lua_infos[0] = cx;
	ent->lua_infos[1] = cy;
	ent->lua_infos[2] = chunk->entities_len - 1;
	ent->friction = entities_infos[id].friction;
	ent->is_moving = 0;
	return ent;
}

void entity_destroy(entity_t *ent) {
	luaL_unref(lua_state, LUA_REGISTRYINDEX, ent->lua_ref);
	ent->lua_ref = LUA_NOREF;
	ent->lua_infos = NULL;
	if (!ent->data.is_lua)
		free(ent->data.c);
	else
		luaL_unref(lua_state, LUA_REGISTRYINDEX, ent->data.lua_ref);
	ent->data.is_lua = 0;
	ent->data.c = NULL;
}
