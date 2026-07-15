#include "game.h"

static uint8_t is_init[UDATA_ID_MAX] = {0};

void *game_new_lua_udata(const char *table_name, size_t table_id, size_t size, int *ref) {
	if (!is_init[table_id]) {
		is_init[table_id] = 1;
		luaL_newmetatable(lua_state, table_name);
		lua_pop(lua_state, 1);
	}
	
	void *res = lua_newuserdata(lua_state, size);
	luaL_getmetatable(lua_state, table_name);
	lua_setmetatable(lua_state, -2);

	*ref = luaL_ref(lua_state, LUA_REGISTRYINDEX);
	return res;
}
