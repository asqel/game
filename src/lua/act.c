#include "game.h"

int lua_func_get_action(lua_State *l) {
	if (lua_gettop(l) != 1)
		return luaL_error(l, "expected 1 argument");
	luaL_checktype(l, 1, LUA_TNUMBER);
	int64_t act = lua_tonumber(l, 1);
	if (act < 0 || act >= GAME_ACT_ENUM_MAX || act == GAME_ACT_NONE)
		return luaL_error(l, "invalide action %d", act);
	lua_pushnumber(l, game_ctx->actions[act]);
	return 1;
}

int lua_func_set_action(lua_State *l) {
	if (lua_gettop(l) != 2)
		return luaL_error(l, "expected 2 argument");
	luaL_checktype(l, 1, LUA_TNUMBER);
	luaL_checktype(l, 2, LUA_TNUMBER);
	int64_t act = lua_tonumber(l, 1);
	int64_t time_act = lua_tonumber(l, 2);
	if (act < 0 || act >= GAME_ACT_ENUM_MAX || act == GAME_ACT_NONE) {
		lua_pushfstring(l, "Error: set_action argument invalide %d", act);
		return lua_error(l);
	}
	game_ctx->actions[act] = time_act;
	return 0;
}
