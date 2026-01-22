#include "game.h"

int lua_func_open_gui(lua_State *l) {
	if (lua_gettop(l) != 4) {
		lua_pushstring(l, "Error: open_gui takes exactly 4 arguments");
		return lua_error(l);
	}
	for (int i = 2; i <= 4; i++) {
		if (lua_type(l, i) != LUA_TFUNCTION) {
			lua_pushfstring(l, "Error: open_gui takes a function as arg #%d", i);
			return lua_error(l);
		}
	}
	c_lua_obj_t data[4];
	for (int i = 1; i <= 4; i++) {
		data[i - 1].is_lua = 1;
		lua_pushvalue(l, i);
		data[i - 1].lua_ref = luaL_ref(l, LUA_REGISTRYINDEX);
	}
	game_open_gui2(data[0], data[1], data[2], data[3]);		

	return 0;
}

int lua_func_close_gui(lua_State *l) {
	if (lua_gettop(l)) {
		lua_pushstring(l, "Error: close_gui doesnt take any argument");
		return lua_error(l);
	}
	game_close_gui();
	return 0;
}
