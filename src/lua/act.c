#include "game.h"

int lua_func_get_action(lua_State *l) {
	if (lua_gettop(l) != 1 || lua_type(l, 1) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: get_action only takes 1 number as argument");
		return lua_error(l);
	}
	int64_t act = lua_tonumber(l, 1);
	if (act < 0 || act >= GAME_ACT_ENUM_MAX || act == GAME_ACT_NONE) {
		lua_pushfstring(l, "Error: get_action argument invalide %d", act);
		return lua_error(l);
	}
	lua_pushnumber(l, game_ctx->actions[act]);
	return 1;
}

int lua_func_set_action(lua_State *l) {
	if (lua_gettop(l) != 2 || lua_type(l, 1) != LUA_TNUMBER || lua_type(l, 2) != LUA_TNUMBER) {
		lua_pushstring(l, "Error: set_action only takes 2 number as argument");
		return lua_error(l);
	}
	int64_t act = lua_tonumber(l, 1);
	int64_t time_act = lua_tonumber(l, 2);
	if (act < 0 || act >= GAME_ACT_ENUM_MAX || act == GAME_ACT_NONE) {
		lua_pushfstring(l, "Error: set_action argument invalide %d", act);
		return lua_error(l);
	}
	game_ctx->actions[act] = time_act;
	return 0;
}
