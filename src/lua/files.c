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
	if (name[0] != '/') {
		lua_pushstring(l, "Error: launch_file path must be absolute");
		return lua_error(l);
	}
	if (strstr(name, "..")) {
		lua_pushstring(l, "Error: launch_file path must not contain '..'");
		return lua_error(l);
	}

	char *path = malloc(strlen(game_dir) + strlen("/scripts") + strlen(name) + 1);
	sprintf(path, "%s/scripts%s", game_dir, name);

	int top_before = lua_gettop(l);

	char *old_path = NULL;
	lua_getglobal(l, "__path__");
	if (lua_type(l,  -1) == LUA_TSTRING)
		old_path = strdup(lua_tostring(l, -1));
	lua_pop(l, 1);

	lua_pushstring(l, name);
	lua_setglobal(l, "__path__");

	if (luaL_dofile(l, path) != LUA_OK) {
		const char *err = lua_tostring(l, -1);
		lua_pop(l, 1);
		lua_pushstring(l, err);
		free(path);
		if (old_path) {
			lua_pushstring(l, old_path);
			lua_setglobal(l, "__path__");
		}
		free(old_path);
		return lua_error(l);
	}
	free(path);
	if (old_path) {
		lua_pushstring(l, old_path);
		lua_setglobal(l, "__path__");
	}
	free(old_path);
	return lua_gettop(l) - top_before;
}
