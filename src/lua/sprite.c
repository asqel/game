#include "game.h"

// name, frame_len_tick, default_state, (texture_ids || texture_name (> 0))...
int lua_func_register_sprite(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc < 4) {
		lua_pushstring(l, "Error: register_sprite takes at least 4 arguments");
		return lua_error(l);	
	}
	if (lua_type(l, 1) != LUA_TSTRING) {
		lua_pushstring(l, "Error: register_sprite takes a string as arg #1");
		return lua_error(l);
	}
	if (lua_type(l, 2) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: register_sprite takes a number as arg #2");
		return lua_error(l);
	}
	if (lua_type(l, 3) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: register_sprite takes a number as arg #3");
		return lua_error(l);
	}
	for (int i = 4; i <= argc; i++) {
		if (lua_type(l, i) != LUA_TNUMBER && lua_type(l, i) != LUA_TSTRING) {
			lua_pushfstring(l, "Error: register_sprite takes a number or a string as arg #%d", i);
			return lua_error(l);
		}
	}
	
	uint32_t *ids = malloc(sizeof(uint32_t) * (argc - 3));
	for (int i = 0; i < argc - 3; i++) {
		if (lua_type(l, i) == LUA_TNUMBER)
			ids[i] = lua_tonumber(l, i + 4);
		else
			ids[i] = game_texture_get_id(lua_tostring(l, i + 4));
	}

	register_sprite(ids, argc - 3, lua_tonumber(l, 2), lua_tostring(l, 1), lua_tonumber(l, 3)); 
	free(ids);

	return 0;
}

int lua_func_get_sprite_id(lua_State *l) {
	if (lua_gettop(l) != 1) {
		lua_pushstring(l, "Error: get_sprite_id takes exactly 1 argument");
		return lua_error(l);
	}
	if (lua_type(l, 1) != LUA_TSTRING) {
		lua_pushstring(l, "Error: get_sprite_id takes only a string as argument");
		return lua_error(l);
	}
	lua_pushnumber(l, get_sprite_id(lua_tostring(l, 1)));
	return 1;
}
