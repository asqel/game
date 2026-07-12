#include "game.h"
/*
register_entity(name, sprite_id, tick, hp, hitx, hity, hitw, hith) -> id
entity_add(id, x, y) -> entity_t (udata)
*/

int lua_func_register_entity(lua_State *l) {
	if (lua_gettop(l) != 8 && lua_gettop(l) != 9)
		return luaL_error(l, "expected 8 to 9 arguments");

	luaL_checktype(l, 1, LUA_TSTRING);
	luaL_checktype(l, 2, LUA_TNUMBER);
	if (!lua_isfunction(l, 3) && !lua_isnil(l, 3))
		return luaL_error(l, "expected a function or nil as arg #3");

	luaL_checktype(l, 4, LUA_TNUMBER);
	luaL_checktype(l, 5, LUA_TNUMBER);
	luaL_checktype(l, 6, LUA_TNUMBER);
	luaL_checktype(l, 7, LUA_TNUMBER);
	luaL_checktype(l, 8, LUA_TNUMBER);

	double friction = luaL_optnumber(l, 9, DEFAULT_FRICTION);

	char *name = (char *)lua_tostring(l, 1);
	int sprite_id = lua_tonumber(l, 2);
	int tick = LUA_REFNIL;
	int hp = lua_tonumber(l, 4);

	double hit[4];
	hit[0] = lua_tonumber(l, 5);
	hit[1] = lua_tonumber(l, 6);
	hit[2] = lua_tonumber(l, 7);
	hit[3] = lua_tonumber(l, 8);

	if (lua_isfunction(l, 3)) {
		lua_pushvalue(l, 3);
		tick = luaL_ref(l, LUA_REGISTRYINDEX);
	}
	lua_pushnumber(l, entity_register(name, sprite_id, tick, hp, hit, friction));
	return 1;
}

int lua_func_entity_add(lua_State *l) {
	if (lua_gettop(l) != 3)
		return luaL_error(l, "expected 3 arguments");
	luaL_checktype(l, 1, LUA_TNUMBER);
	luaL_checktype(l, 2, LUA_TNUMBER);
	luaL_checktype(l, 3, LUA_TNUMBER);

	uint32_t id = lua_tonumber(l, 1);
	int x = lua_tonumber(l, 2);
	int y = lua_tonumber(l, 3);

	entity_t *ent = entity_add(id, x, y, game_ctx->world);
	lua_rawgeti(l, LUA_REGISTRYINDEX, ent->lua_ref);
	return 1;
}

int lua_func_set_entity_func(lua_State *l) {
	if (lua_gettop(l) != 2)
		return luaL_error(l, "expected 2 arguments");
	luaL_checktype(l, 1, LUA_TNUMBER);
	if (!lua_isfunction(l, 2) && !lua_isnil(l, 2))
		return luaL_error(l, "expected a function or nil as arg #2");
	uint32_t id = lua_tonumber(l, 1);
	if (id >= (uint32_t)entities_infos_len)
		return luaL_error(l, "entity id invalid (%d)", id);
	lua_pushvalue(l, 2);
	int ref = luaL_ref(l, LUA_REGISTRYINDEX);
	lua_rawgeti(l, LUA_REGISTRYINDEX, entities_infos[id].on_tick_ref);
	luaL_unref(l, LUA_REGISTRYINDEX, entities_infos[id].on_tick_ref);
	entities_infos[id].on_tick_ref = ref;
	return 1;
}

// entity, velx, vely
int lua_func_set_entity_velocity(lua_State *l) {
	if (lua_gettop(l) != 3)
		return luaL_error(l, "expected 3 arguments");
	int *entity = luaL_checkudata(l, 1, "entity_t");
	luaL_checktype(l, 2, LUA_TNUMBER);
	luaL_checktype(l, 3, LUA_TNUMBER);

	entity_t * ent = &game_ctx->world->chunks[entity[1]][entity[0]]->entities[entity[2]];
	ent->vx = lua_tonumber(l, 2);
	ent->vy = lua_tonumber(l, 3);
	return 0;
}

int lua_func_get_entity_ismoving(lua_State *l) {
	if (lua_gettop(l) != 1)
		return luaL_error(l, "expected 1 argument");
	int *ent_info = luaL_checkudata(l, 1, "entity_t");
	entity_t *ent = &game_ctx->world->chunks[ent_info[1]][ent_info[0]]->entities[ent_info[2]];
	lua_pushnumber(l, ent->is_moving);
	return 1;
}

// !TODO set/get entity data
int lua_func_set_entity_data(lua_State *l) {
	if (lua_gettop(l) != 2)
		return luaL_error(l, "expected 2 argument");
	int *ent_info = luaL_checkudata(l, 1, "entity_t");
	entity_t *ent = &game_ctx->world->chunks[ent_info[1]][ent_info[0]]->entities[ent_info[2]];
	lua_pushvalue(l, 2);
	int ref = luaL_ref(l, LUA_REGISTRYINDEX);
	luaL_unref(l, LUA_REGISTRYINDEX, ent->data_ref);
	ent->data_ref = ref;
	return 0;
}
