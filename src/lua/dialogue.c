#include "game.h"

/*
name: str,
on_end: func | nil,
...: string (>= 1)
*/
int lua_func_open_dialogue(lua_State *L) {
	const char *name = NULL;	
	int on_end = LUA_NOREF;
	int dialogues_len = 0;

	if (lua_gettop(L) < 3) {
		lua_pushstring(L, "Error: open_dialogue takes at least 3 arguments\n");
		return lua_error(L);
	}
	if (lua_type(L, 1) != LUA_TSTRING) {
		lua_pushstring(L, "Error: open_dialogue takes a string as arg #1\n");
		return lua_error(L);
	}
	name = lua_tostring(L, 1);
	if (strlen(name) > 511) {
		lua_pushstring(L, "Error: open_dialogue takes a string as arg #1 with a max len of 511\n");
		return lua_error(L);
		
	}
	
	int arg_len = lua_gettop(L);
	dialogues_len = arg_len - 2;
	for (int i = 3; i <= arg_len; i++) {
		if (lua_type(L, i) != LUA_TSTRING) {
			lua_pushfstring(L, "Error: open_dialogue takes a string as arg #%d", i);
			return lua_error(L);
		}
	}
	
	if (lua_type(L, 2) == LUA_TFUNCTION)	
		on_end = luaL_ref(L, LUA_REGISTRYINDEX);
	else if (lua_type(L, 2) != LUA_TNIL) {
		lua_pushstring(L, "Error: open_dialogue takes a function or nil as arg #2\n");
		return lua_error(L);
	}

	size_t *ids = malloc(sizeof(size_t) * dialogues_len);
	for (int i = 3; i <= arg_len; i++)
		ids[i - 3] = get_dialogue_id(lua_tostring(L, i));

	c_lua_obj_t on_end_obj = {0};
	if (on_end != LUA_NOREF) {
		on_end_obj.is_lua = 1;
		on_end_obj.lua_ref = on_end;
	}

	open_dialogue2((char *)name, on_end_obj, dialogues_len, ids);
	free(ids);
	return 0;
}
