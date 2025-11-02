#include "game.h"

int interact_at(int x, int y) {
	obj_t *obj = world_get_obj_at(game_ctx->world, x, y, 1);
	if (obj && obj->id > 0 && obj->id < obj_registry_len) {
		c_lua_obj_t interact = obj_registry[obj->id].interact;
		if (interact.is_lua) {
			lua_rawgeti(lua_state, LUA_REGISTRYINDEX, interact.lua_ref);
			lua_pushinteger(lua_state, x);
			lua_pushinteger(lua_state, y);
	
			if (lua_pcall(lua_state, 2, 0, 0) != LUA_OK) {
				PRINT_ERR("Erreur: Lua : %s\n", lua_tostring(lua_state, -1));
				lua_pop(lua_state, 1);
			}
			return 1;
		}
		else if (interact.c) {
			void (*func)(int, int) = ((void (*)(int, int))interact.c);
			func(x, y);
			return 1;
		}
	}
	return 0;
}

void player_interact() {
	int x = game_ctx->player->x;
	int y = game_ctx->player->y;

	if (game_ctx->player->dir == 0)
		y--;
	else if (game_ctx->player->dir == 1)
		x++;
	else if (game_ctx->player->dir == 2)
		y++;
	else if (game_ctx->player->dir == 3)
		x--;
	if (!interact_at(game_ctx->player->x, game_ctx->player->y))
		interact_at(x, y);
}
