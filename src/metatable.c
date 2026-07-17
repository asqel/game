#include "game.h"

void *game_new_lua_udata(const char *table_name, size_t size, int *ref) {
	void *res = lua_newuserdata(lua_state, size);

	luaL_getmetatable(lua_state, table_name);
	lua_setmetatable(lua_state, -2);

	*ref = luaL_ref(lua_state, LUA_REGISTRYINDEX);
	return res;
}
