#include "game.h"

int lua_func_time(lua_State *l) {
	if (lua_gettop(l) != 0) {
		lua_pushstring(l, "Error: time doesnst take any argument");
		return lua_error(l);
	}
	lua_pushnumber(l, game_get_time());
	return 1;
}
