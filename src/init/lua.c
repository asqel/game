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
	add_lua_func(lua_func_launch_file, "dofile");
	add_lua_func(lua_func_open_dialogue, "open_dialogue");
	add_lua_func(lua_func_open_gui, "open_gui");
	add_lua_func(lua_func_draw_text, "draw_text");
	add_lua_number(GAME_ACT_UP, "GAME_ACT_UP");
	add_lua_number(GAME_ACT_RIGHT, "GAME_ACT_RIGHT");
	add_lua_number(GAME_ACT_DOWN, "GAME_ACT_DOWN");
	add_lua_number(GAME_ACT_LEFT, "GAME_ACT_LEFT");
	add_lua_number(GAME_ACT_INTERACT, "GAME_ACT_INTERACT");
	add_lua_number(GAME_ACT_CLOSE, "GAME_ACT_CLOSE");
	add_lua_number(GAME_ACT_EDIT_UP, "GAME_ACT_EDIT_UP");
	add_lua_number(GAME_ACT_EDIT_RIGHT, "GAME_ACT_EDIT_RIGHT");
	add_lua_number(GAME_ACT_EDIT_DOWN, "GAME_ACT_EDIT_DOWN");
	add_lua_number(GAME_ACT_EDIT_LEFT, "GAME_ACT_EDIT_LEFT");
	add_lua_number(GAME_ACT_EDIT_PLACE, "GAME_ACT_EDIT_PLACE");
	add_lua_number(GAME_ACT_EDIT_REMOVE, "GAME_ACT_EDIT_REMOVE");
	add_lua_number(GAME_ACT_EDIT_FILL, "GAME_ACT_EDIT_FILL");
	add_lua_number(GAME_ACT_EDIT_EMPTY, "GAME_ACT_EDIT_EMPTY");
	add_lua_number(GAME_ACT_EDIT_SAVE, "GAME_ACT_EDIT_SAVE");
	add_lua_func(lua_func_time, "time");
	add_lua_func(lua_func_set_action, "set_action");
	add_lua_func(lua_func_get_action, "get_action");
	add_lua_func(lua_func_close_gui, "close_gui");
}

static void load_libs() {
	luaL_requiref(lua_state, "_G", luaopen_base, 1);
	lua_pop(lua_state, 1);

	luaL_requiref(lua_state, LUA_MATHLIBNAME, luaopen_math, 1);
	lua_pop(lua_state, 1);

	luaL_requiref(lua_state, LUA_STRLIBNAME, luaopen_string, 1);
	lua_pop(lua_state, 1);

	luaL_requiref(lua_state, LUA_TABLIBNAME, luaopen_table, 1);
	lua_pop(lua_state, 1);
}


int init_lua() {
	lua_state = luaL_newstate();
	if (!lua_state) {
		PRINT_ERR("Error: couldn't open lua state\n");
		return 1;
	}

	load_libs();
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
