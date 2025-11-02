#include "game.h"

int print_c(lua_State *state) {
	printf("hellow from c from lua\n");
	return 0;
}

void add_funcs() {
	lua_pushcfunction(lua_state, &print_c);
	lua_setglobal(lua_state, "le_print");
}

int init_lua() {
	lua_state = luaL_newstate();
	if (!lua_state) {
		PRINT_ERR("Error: couldn't open lua state\n");
		return 1;
	}

	luaL_openlibs(lua_state);
	add_funcs();
	char *path = malloc(strlen(game_dir) + strlen("/scripts/main.lua") + 1);
	sprintf(path, "%s/scripts/main.lua", game_dir);
	if (luaL_dofile(lua_state, path) != LUA_OK) {
		PRINT_ERR("Error lua: %s\n", lua_tostring(lua_state, -1));
		free(path);
		lua_close(lua_state);
		return 1;
	}
	free(path);
	return 0;
}
