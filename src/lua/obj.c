#include "game.h"

int lua_func_register_obj(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc < 4) {
		lua_pushstring(l, "Error: register_obj takes at least 4 argument");
		return lua_error(l);
	}
	if (lua_type(l, 1) != LUA_TSTRING) {
		lua_pushstring(l, "Error: register_obj takes a string as arg #1");
		return lua_error(l);
	}
	if (lua_type(l, 2) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: register_obj takes a string as arg #2");
		return lua_error(l);
	}
	if (lua_type(l, 3) != LUA_TFUNCTION && lua_type(l, 3) != LUA_TNIL) {
		lua_pushstring(l, "Error: register_obj takes a function or nil as arg #3");
		return lua_error(l);
	}
	if (lua_type(l, 4) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: register_obj takes a number as arg #4");
		return lua_error(l);
	}
	char *name = (char *)lua_tostring(l, 1);
	int sprite_id = lua_tonumber(l, 2);
	int has_hitbox = lua_tonumber(l, 4);
	c_lua_obj_t func;
	func.is_lua = 0;
	func.c = NULL;
	if (lua_type(l, 3) == LUA_TFUNCTION) {
		lua_pushvalue(l, 3);
		int ref = luaL_ref(l, LUA_REGISTRYINDEX);
		func.is_lua = 1;
		func.lua_ref = ref;
	}
	game_register_obj_full(name, sprite_id, func, has_hitbox, 0, 0, 0, 0);	
	return 0;
}

int lua_func_register_obj2(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc < 4) {
		lua_pushstring(l, "Error: register_obj2 takes at least 4 argument");
		return lua_error(l);
	}
	if (lua_type(l, 1) != LUA_TSTRING) {
		lua_pushstring(l, "Error: register_obj2 takes a string as arg #1");
		return lua_error(l);
	}
	if (lua_type(l, 2) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: register_obj2 takes a string as arg #2");
		return lua_error(l);
	}
	if (lua_type(l, 3) != LUA_TFUNCTION && lua_type(l, 3) != LUA_TNIL) {
		lua_pushstring(l, "Error: register_obj2 takes a function or nil as arg #3");
		return lua_error(l);
	}
	for (int i = 0; i < 4; i++) {
		if (lua_type(l, 4 + i) != LUA_TNUMBER) {
			lua_pushfstring(l, "Error: register_obj2 takes a number as arg #%d", 4 + i);
			return lua_error(l);
		}
	}
	char *name = (char *)lua_tostring(l, 1);
	int sprite_id = lua_tonumber(l, 2);
	c_lua_obj_t func;
	func.is_lua = 0;
	func.c = NULL;
	if (lua_type(l, 3) == LUA_TFUNCTION) {
		lua_pushvalue(l, 3);
		int ref = luaL_ref(l, LUA_REGISTRYINDEX);
		func.is_lua = 1;
		func.lua_ref = ref;
	}
	game_register_obj_full(name, sprite_id, func, 2, lua_tonumber(l, 4), lua_tonumber(l, 5), lua_tonumber(l, 6), lua_tonumber(l, 7));	
	return 0;
}
