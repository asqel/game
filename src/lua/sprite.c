#include "game.h"

// name, frame_len_tick, default_state, (texture_ids || texture_name (> 0))...
int lua_func_register_sprite(lua_State *l) {
	int argc = lua_gettop(l);
	if (argc < 4)
		return luaL_error(l, "expected at least 4 arguments");
	luaL_checktype(l, 1, LUA_TSTRING);
	luaL_checktype(l, 2, LUA_TNUMBER);
	luaL_checktype(l, 3, LUA_TNUMBER);
	for (int i = 4; i <= argc; i++) {
		if (!lua_isstring(l, i) && !lua_isnumber(l, i))
			return luaL_error(l, "expected a number or a string as arg #%d", i);
	}
	
	uint32_t *ids = malloc(sizeof(uint32_t) * (argc - 3));
	for (int i = 0; i < argc - 3; i++) {
		if (lua_type(l, i + 4) == LUA_TNUMBER)
			ids[i] = lua_tonumber(l, i + 4);
		else
			ids[i] = game_texture_get_id(lua_tostring(l, i + 4));
	}

	register_sprite(ids, argc - 3, lua_tonumber(l, 2), lua_tostring(l, 1), lua_tonumber(l, 3)); 
	free(ids);

	return 0;
}

int lua_func_get_sprite_id(lua_State *l) {
	if (lua_gettop(l) != 1)
		return luaL_error(l, "expected 1 arguments");
	luaL_checktype(l, 1, LUA_TSTRING);
	lua_pushnumber(l, get_sprite_id(lua_tostring(l, 1)));
	return 1;
}
