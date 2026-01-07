#include "game.h"

static void add_lua_func(void *f, char *name) {
	lua_pushcfunction(lua_state, f);
	lua_setglobal(lua_state, name);	
}

static void add_lua_number(int n, char *name) {
	lua_pushnumber(lua_state, n);
	lua_setglobal(lua_state, name);	
}

static void init_lua_funcs() {
	add_lua_number(SPRITE_MASK_PAUSED, "SPRITE_PAUSED");
	add_lua_number(SPRITE_MASK_LOOP, "SPRITE_LOOP");
	add_lua_func(lua_func_register_obj, "register_obj");
	add_lua_func(lua_func_register_obj2, "register_obj2");
	add_lua_func(lua_func_register_sprite, "register_sprite");
	add_lua_func(lua_func_get_sprite_id, "get_sprite_id");
	add_lua_func(lua_func_launch_file, "launch_file");
}

int init_lua() {
	lua_state = luaL_newstate();
	if (!lua_state) {
		PRINT_ERR("Error: couldn't open lua state\n");
		return 1;
	}

	luaL_openlibs(lua_state);
	init_lua_funcs();

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
