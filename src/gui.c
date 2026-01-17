#include "game.h"

void game_tick_gui() {
	gui_t *gui = game_ctx->player->gui;
	if (!gui)
		return ;
		
	if (gui->update.is_lua) {
		lua_rawgeti(lua_state, LUA_REGISTRYINDEX, gui->update.lua_ref);
		if (lua_pcall(lua_state, 0, 0, 0) !=  LUA_OK) {
			PRINT_ERR("Erreur: Lua : %s\n", lua_tostring(lua_state, -1));
			lua_pop(lua_state, 1);
		}
	}
	else if (gui->update.c)
		((void (*)(gui_t *))gui->update.c)(gui);
}

void game_close_gui() {
	if (!game_ctx->player->gui)
		return ;
	gui_t *gui = game_ctx->player->gui;

	if (gui->free.is_lua) {
		lua_rawgeti(lua_state, LUA_REGISTRYINDEX, gui->free.lua_ref);
		if (lua_pcall(lua_state, 0, 0, 0) !=  LUA_OK) {
			PRINT_ERR("Erreur: Lua : %s\n", lua_tostring(lua_state, -1));
			lua_pop(lua_state, 1);
		}
	}
	else if (gui->free.c)
		((void (*)(gui_t *))gui->free.c)(gui);

	if (gui->data.is_lua)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, gui->data.lua_ref);
	if (gui->update.is_lua)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, gui->update.lua_ref);
	if (gui->render.is_lua)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, gui->render.lua_ref);
	if (gui->free.is_lua)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, gui->free.lua_ref);
	

	free(game_ctx->player->gui);
	game_ctx->player->gui = NULL;
}

void game_render_gui() {
	if (!game_ctx->player->gui)
		return ;
	gui_t *gui = game_ctx->player->gui;
	if (gui->render.is_lua) {
		lua_rawgeti(lua_state, LUA_REGISTRYINDEX, gui->render.lua_ref);
		if (lua_pcall(lua_state, 0, 0, 0) !=  LUA_OK) {
			PRINT_ERR("Erreur: Lua : %s\n", lua_tostring(lua_state, -1));
			lua_pop(lua_state, 1);
		}
	}
	else if (gui->render.c)
		((void (*)(gui_t *))gui->render.c)(gui);
}

gui_t *game_open_gui(void *data, void (*update)(gui_t *self), void (*render)(gui_t *self), void (*free)(gui_t *self)) {
	gui_t *gui = malloc(sizeof(gui_t));
	*gui = (gui_t){0};
	gui->data.c = data;
	gui->free.c = free;
	gui->render.c = render;
	gui->update.c = update;
	game_close_gui();
	game_ctx->player->gui = gui;
	for (int i = 0; i < GAME_ACT_ENUM_MAX; i++)
		game_ctx->actions[i] = 0;
	return gui;
}