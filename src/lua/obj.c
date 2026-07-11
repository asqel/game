#include "game.h"

int lua_func_register_obj(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc < 3) {
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
	if (lua_type(l, 3) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: register_obj takes a number as arg #4");
		return lua_error(l);
	}
	char *name = (char *)lua_tostring(l, 1);
	int sprite_id = lua_tonumber(l, 2);
	int has_hitbox = lua_tonumber(l, 3);
	register_obj(name, sprite_id, has_hitbox, 0, 0, 0, 0);	
	return 0;
}

int lua_func_register_obj2(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc < 3) {
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
	for (int i = 0; i < 3; i++) {
		if (lua_type(l, 3 + i) != LUA_TNUMBER) {
			lua_pushfstring(l, "Error: register_obj2 takes a number as arg #%d", 3 + i);
			return lua_error(l);
		}
	}
	char *name = (char *)lua_tostring(l, 1);
	int sprite_id = lua_tonumber(l, 2);
	register_obj(name, sprite_id, 2, lua_tonumber(l, 3), lua_tonumber(l, 4), lua_tonumber(l, 5), lua_tonumber(l, 6));	
	return 0;
}
