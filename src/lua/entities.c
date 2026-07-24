#include "game.h"
/*
register_entity(name, sprite_id, tick, hp, hitx, hity, hitw, hith) -> id
entity_add(id, x, y) -> entity_t (udata)
*/

int lua_func_register_entity(lua_State *l) {
	if (lua_gettop(l) < 8 || 10 < lua_gettop(l))
		return luaL_error(l, "expected 8 to 10 arguments");

	luaL_checktype(l, 1, LUA_TSTRING);
	luaL_checktype(l, 2, LUA_TNUMBER);
	if (!lua_isfunction(l, 3) && !lua_isnil(l, 3))
		return luaL_error(l, "expected a function or nil as arg #3");

	luaL_checktype(l, 4, LUA_TNUMBER);
	luaL_checktype(l, 5, LUA_TNUMBER);
	luaL_checktype(l, 6, LUA_TNUMBER);
	luaL_checktype(l, 7, LUA_TNUMBER);
	luaL_checktype(l, 8, LUA_TNUMBER);

	double drag = luaL_optnumber(l, 9, DEFAULT_FRICTION);
	int8_t sprite_depth = luaL_optnumber(l, 10, 0);

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
	lua_pushnumber(l, entity_register(name, sprite_id, tick, hp, hit, drag, sprite_depth));
	return 1;
}

int lua_func_entity_add(lua_State *l) {
	if (lua_gettop(l) != 3)
		return luaL_error(l, "expected 3 arguments");
	luaL_checktype(l, 1, LUA_TNUMBER);
	luaL_checktype(l, 2, LUA_TNUMBER);
	luaL_checktype(l, 3, LUA_TNUMBER);

	uint32_t id = lua_tonumber(l, 1);
	double x = lua_tonumber(l, 2);
	double y = lua_tonumber(l, 3);

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

static int get_set_ref(int *dest, int is_getting, int idx, lua_State *l) {
	if (is_getting) {
		lua_rawgeti(l, LUA_REGISTRYINDEX, *dest);
		return 1;
	}
	lua_pushvalue(l, idx);
	int ref = luaL_ref(l, LUA_REGISTRYINDEX);
	luaL_unref(l, LUA_REGISTRYINDEX, *dest);
	*dest = ref;
	return 0;
}

static int get_set_double(double *dest, int is_getting, int idx, lua_State *l) {
	if (is_getting) {
		lua_pushnumber(l, *dest);
		return 1;
	}

	luaL_checktype(l, idx, LUA_TNUMBER);
	*dest = lua_tonumber(l, idx);
	return 0;
}

static int get_set_i8(int8_t *dest, int is_getting, int idx, lua_State *l) {
	if (is_getting) {
		lua_pushnumber(l, *dest);
		return 1;
	}

	luaL_checktype(l, idx, LUA_TNUMBER);
	*dest = lua_tonumber(l, idx);
	return 0;
}

int lua_func_entity_meta_index(lua_State *l) {
	int is_getting = (lua_gettop(l) == 2);

	luaL_checktype(l, 2, LUA_TSTRING);
	const char *member = lua_tostring(l, 2);
	int *entity = luaL_checkudata(l, 1, "entity_t");
	entity_t *ent = NULL;
	if (entity[0] == -1)
		return 0;
	ent = &game_ctx->world->chunks[entity[1]][entity[0]]->entities[entity[2]];

	
	if (!strcmp(member, "vx"))
		return get_set_double(&ent->vx, is_getting, 3, l);
	if (!strcmp(member, "vy"))
		return get_set_double(&ent->vy, is_getting, 3, l);
	if (!strcmp(member, "drag"))
		return get_set_double(&ent->drag, is_getting, 3, l);
	if (!strcmp(member, "world_hitbox_x"))
		return get_set_double(&ent->world_hitbox_x, is_getting, 3, l);
	if (!strcmp(member, "world_hitbox_y"))
		return get_set_double(&ent->world_hitbox_y, is_getting, 3, l);
	if (!strcmp(member, "world_hitbox_w"))
		return get_set_double(&ent->world_hitbox_w, is_getting, 3, l);
	if (!strcmp(member, "world_hitbox_h"))
		return get_set_double(&ent->world_hitbox_h, is_getting, 3, l);
	if (!strcmp(member, "hp"))
		return get_set_double(&ent->hp, is_getting, 3, l);
	if (!strcmp(member, "data"))
		return get_set_ref(&ent->data_ref, is_getting, 3, l);
	if (!strcmp(member, "depth"))
		return get_set_i8(&ent->sprite_depth, is_getting, 3, l);
	if (!strcmp(member, "x"))
		return get_set_double(&ent->x, 1, 3, l);
	if (!strcmp(member, "y"))
		return get_set_double(&ent->y, 1, 3, l);

	return luaL_error(l, "unknown member %s in type entity_t", member);
}
