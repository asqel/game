#include "game.h"

int lua_func_launch_file(lua_State *l) {
	if (lua_gettop(l) != 1) {
		lua_pushstring(l, "Error: launch_file takes exactly 1 argument");
		return lua_error(l);
	}
	if (lua_type(l, 1) != LUA_TSTRING) {
		lua_pushstring(l, "Error: launch_file takes only a string as argument");
		return lua_error(l);
	}
	const char *name = lua_tostring(l, 1);
	char *path = malloc(strlen(game_dir) + strlen("/scripts/") + strlen(name) + 1);
	sprintf(path, "%s/scripts/%s", game_dir, name);
	if (luaL_dofile(l, path) != LUA_OK)
		PRINT_ERR("Error lua: %s\n", lua_tostring(l, -1));
	free(path);
	return 0;
}
