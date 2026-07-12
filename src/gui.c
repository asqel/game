#include "game.h"

void game_tick_gui() {
	gui_t *gui = game_ctx->player->gui;
	if (!gui)
		return ;
	if (gui->update_ref == LUA_REFNIL)
		return ;
		
	lua_rawgeti(lua_state, LUA_REGISTRYINDEX, gui->update_ref);
	if (lua_pcall(lua_state, 0, 0, 0) !=  LUA_OK) {
		PRINT_ERR("Erreur: Lua : %s\n", lua_tostring(lua_state, -1));
		lua_pop(lua_state, 1);
	}
}

void game_close_gui() {
	if (!game_ctx->player->gui)
		return ;
	gui_t *gui = game_ctx->player->gui;

	if (gui->free_ref) {
		lua_rawgeti(lua_state, LUA_REGISTRYINDEX, gui->free_ref);
		if (lua_pcall(lua_state, 0, 0, 0) !=  LUA_OK) {
			PRINT_ERR("Erreur: Lua : %s\n", lua_tostring(lua_state, -1));
			lua_pop(lua_state, 1);
		}
	}

	luaL_unref(lua_state, LUA_REGISTRYINDEX, gui->data_ref);
	luaL_unref(lua_state, LUA_REGISTRYINDEX, gui->update_ref);
	luaL_unref(lua_state, LUA_REGISTRYINDEX, gui->render_ref);
	luaL_unref(lua_state, LUA_REGISTRYINDEX, gui->free_ref);
	

	free(game_ctx->player->gui);
	game_ctx->player->gui = NULL;
}

void game_render_gui() {
	if (!game_ctx->player->gui)
		return ;
	gui_t *gui = game_ctx->player->gui;
	if (gui->render_ref == LUA_REFNIL)
	return ;
	lua_rawgeti(lua_state, LUA_REGISTRYINDEX, gui->render_ref);
	if (lua_pcall(lua_state, 0, 0, 0) !=  LUA_OK) {
		PRINT_ERR("Erreur: Lua : %s\n", lua_tostring(lua_state, -1));
		lua_pop(lua_state, 1);
	}
}

gui_t *game_open_gui(int data_ref, int update_ref, int render_ref, int free_ref) {
	gui_t *gui = malloc(sizeof(gui_t));
	*gui = (gui_t){0};
	gui->data_ref = data_ref;
	gui->free_ref = free_ref;
	gui->render_ref = render_ref;
	gui->update_ref = update_ref;
	game_close_gui();
	game_ctx->player->gui = gui;
	for (int i = 0; i < GAME_ACT_ENUM_MAX; i++)
		game_ctx->actions[i] = 0;
	return gui;
}