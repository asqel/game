#include "game.h"

int init_lua() {
	lua_state = luaL_newstate();
	if (!lua_state) {
		PRINT_ERR("Error: couldn't open lua state\n");
		return 1;
	}

	luaL_openlibs(lua_state);
	return 0;
}
