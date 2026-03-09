#include "game.h"
/*
register_entity(name, sprite_id, tick, hp, hitx, hity, hitw, hith) -> id
entity_add(id, x, y) -> entity_t (udata)
*/

int lua_func_register_entity(lua_State *l) {
	if (lua_gettop(l) != 8 && lua_gettop(l) != 9) {
		lua_pushstring(l, "Error: register_entity only takes 8 to 9 arguments");
		return lua_error(l);
	}
	if (lua_type(l, 1) != LUA_TSTRING) {
		lua_pushstring(l, "Error: registr_entity only takes a string as arg #1");
		return lua_error(l);
	}
	if (lua_type(l, 2) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: registr_entity only takes a number as arg #2");
		return lua_error(l);
	}
	if (lua_type(l, 3) != LUA_TFUNCTION && lua_type(l, 3) != LUA_TNIL) {
		lua_pushstring(l, "Error: registr_entity only takes a function or nil as arg #3");
		return lua_error(l);
	}
	for (int i = 0; i < 5; i++) {
		if (lua_type(l, 4 + i) != LUA_TNUMBER) {
			lua_pushfstring(l, "Error: register_entity only takes a number as arg #%d", i + 4);
			return lua_error(l);
		}
	}
	double friction = DEFAULT_FRICTION;
	if (lua_gettop(l) == 9) {
		if (lua_type(l, 9) != LUA_TNUMBER) {
			lua_pushfstring(l, "Error: register_entity only takes a number as arg #9");
			return lua_error(l);
		}
		friction = lua_tonumber(l, 9);
	}
	char *name = (char *)lua_tostring(l, 1);
	int sprite_id = lua_tonumber(l, 2);
	c_lua_obj_t tick = (c_lua_obj_t){0};
	int hp = lua_tonumber(l, 4);
	int hit[4] = {0};
	for (int i = 0; i < 4; i++)
		hit[i] = lua_tonumber(l, 5 + i);
	if (lua_type(l, 3) == LUA_TFUNCTION) {
		lua_pushvalue(l, 3);
		tick.lua_ref = luaL_ref(l, LUA_REGISTRYINDEX);
		tick.is_lua = 1;
	}
	lua_pushnumber(l, entity_register(name, sprite_id, tick, hp, hit, friction));
	return 1;
}

int lua_func_entity_add(lua_State *l) {
	if (lua_gettop(l) != 3) {
		lua_pushstring(l, "Error: entity_add only takes 3 arguments");
		return lua_error(l);
	}
	for (int i = 0; i < 3; i++) {
		if (lua_type(l, i + 1) != LUA_TNUMBER) {
			lua_pushfstring(l, "Error: entity_ddd only takes a number as arg #%d", i + 1);
			return lua_error(l);
		}
	}
	lua_rawgeti(l, LUA_REGISTRYINDEX, entity_add(lua_tonumber(l, 1), lua_tonumber(l, 2), lua_tonumber(l, 3), game_ctx->world)->lua_ref);
	return 1;
}
