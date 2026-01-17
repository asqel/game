#include "game.h"

typedef struct dialogue_gui_info_t {
	size_t *dialogue_ids;
	size_t len;
	//	int (*on_end)(gui_t *self); // returns 1 if no close
	c_lua_obj_t on_end;
	char name[512]; // name above dialogue box

	size_t current_idx;
	size_t current_len;
	size_t current_sublen;

	int sleep; // to time to sleep
	int wait; // do we wait for interact
	size_t style;

	int x;
	int y;

	int width;
	int height;
	
	size_t font;
} dialogue_gui_info_t;

void on_dialogue_free(gui_t *self) {
	dialogue_gui_info_t *data = self->data.c;
	
	if (data->on_end.is_lua)
		luaL_unref(lua_state, LUA_REGISTRYINDEX, data->on_end.lua_ref);	
	free(data->dialogue_ids);
	free(data);
}

void on_dialogue_render(gui_t *self) {
	dialogue_gui_info_t *data = self->data.c;
	
	if (data->current_idx >= data->len)
		return ;
	dialogue_info_t *dialogue = get_dialogue_by_id(data->dialogue_ids[data->current_idx]);  
	display_dialogue(dialogue, data->current_len, data->current_sublen, data->width, data->height, data->style, data->x, data->y, data->font);
}

static void try_end(gui_t *self) {
	dialogue_gui_info_t *data = self->data.c;

	if (!data->on_end.is_lua) {
	 	if (!data->on_end.c)
			game_close_gui();
		else {
			int res = ((int (*)(gui_t *))data->on_end.c)(self);
			if (!res)
				game_close_gui();
		}
	 }
	 else {
	 	lua_rawgeti(lua_state, LUA_REGISTRYINDEX, data->on_end.lua_ref);
		if (lua_pcall(lua_state, 0, 1, 0) != LUA_OK) {
			const char *err = lua_tostring(lua_state, -1);
			PRINT_ERR("Error(gui dialog on end):\n\tlua:%s\n", err);
			game_close_gui();
			return ;
		}
		int res = lua_toboolean(lua_state, -1);
		lua_pop(lua_state, 1);
		if (!res)
			game_close_gui();
	 }
}

static int compute_size_text(dialogue_info_t *dialogue, dialogue_char_t *c, dialogue_gui_info_t *data) {
	char *text = &dialogue->original[c->text[0]];	
	size_t text_len = c->text[1];

	char old_end = text[text_len];
	text[text_len] = '\0';
	
	SDL_Surface *surface = TTF_RenderUTF8_Solid(game_ctx->fonts[0], text, (SDL_Color){255 , 255, 255, 255});
	data->width += surface->w;
	int res = surface->h;

	SDL_FreeSurface(surface);
	text[text_len] = old_end;
	return res;
}

static int compute_size_emote(dialogue_char_t *c, dialogue_gui_info_t *data) {
	char emote_name[512] = {0};
	sprintf(emote_name, "emote_%lu", c->emote);

	int id = game_texture_get_id(emote_name);
	if (id == -1)
		return 0;
	

	SDL_Surface *emote_text = texture_registry[id].surface;
	data->width = emote_text->w;

	return emote_text->h;
}

static void compute_size(dialogue_gui_info_t *data) {
	dialogue_info_t *dialogue = get_dialogue_by_id(data->dialogue_ids[data->current_idx]);  

	size_t i = 0;
	int max_width = 0;
	while (i < dialogue->len) {
		data->width = 0;
		size_t line_len = 0;
		while (i + line_len < dialogue->len) {
			dialogue_char_t *c = &dialogue->text[i + line_len];
			if (c->type == DIALOG_LINE)
				break;
			line_len++;
		}
		int max_h = 0;
		for (size_t k = 0; k < line_len; k++) {
			dialogue_char_t *c = &dialogue->text[i + k];
			int new_h = 0;
			if (c->type == DIALOG_TEXT)
				new_h = compute_size_text(dialogue, c, data);
			else if (c->type == DIALOG_EMOTE)
				new_h = compute_size_emote(c, data);
			if (new_h > max_h)
				max_h = new_h;
		}
		if (max_width < data->width)
			max_width = data->width;
		data->height += game_ctx->fonts_height[data->font];
		i += line_len + 1;
	}
	data->width = max_width;
}

static void skip_special(dialogue_info_t *dialogue, dialogue_gui_info_t *data) {
	while (data->current_len < dialogue->len) {
		dialogue_char_t *c = &dialogue->text[data->current_len];

		if (c->type == DIALOG_PX)
			data->x = c->px;
		else if (c->type == DIALOG_PY)
			data->y = c->py;
		else if (c->type == DIALOG_STYLE)
			data->style = c->style;
		else if (c->type == DIALOG_FONT)
			data->font = c->font;
		else
			break;

		data->current_len++;
	}
}

void on_dialogue_update(gui_t *self) {
	dialogue_gui_info_t *data = self->data.c;

	if (data->sleep) {
		data->sleep--;
		return ;
	}
	if (data->wait) {
		if (!game_ctx->actions[GAME_ACT_INTERACT])
			return ;
		data->wait = 0;
	}
	if (data->current_idx >= data->len) {
		try_end(self);
		return ;
	}

	dialogue_info_t *dialogue = get_dialogue_by_id(data->dialogue_ids[data->current_idx]);  
	if (data->current_len >= dialogue->len) {
		data->current_len = 0;
		data->current_idx++;
		data->current_sublen = 0;
		data->width = 0;
		data->height = 0;
		return ;
	}

	skip_special(dialogue, data);
	if (data->width == 0 || data->height == 0)
		compute_size(data);

	size_t to_read = 1;
	while (to_read && data->current_len < dialogue->len) {
		dialogue_char_t *c = &dialogue->text[data->current_len];	
		if (c->type == DIALOG_TEXT) {
			if (data->current_sublen == c->text[1]) {
				data->current_sublen = 0;
				data->current_len++;
				to_read--;
				continue;
			}
			to_read--;
			data->current_sublen++;
			continue;
		}
		if (c->type == DIALOG_VAR) {
			char *var = "(NULL)";	
			if (c->var < game_ctx->env_len)
				var = game_ctx->env_vars[c->var];
			if (data->current_sublen >= strlen(var)) {
				data->current_sublen = 0;
				data->current_len++;
				to_read--;
				continue;
			}
			data->current_sublen++;
			to_read--;
			continue;
		}
		if (c->type == DIALOG_PAUSE)
			data->sleep = c->pause;
		if (c->type == DIALOG_WAIT)
			data->wait = 1;
		data->current_len++;
	}
}

void open_dialogue(char *name, int (*on_end)(gui_t *self), size_t id_len, ...) {
	if (strlen(name) > 511)
		return ;

	dialogue_gui_info_t *data = calloc(1, sizeof(dialogue_gui_info_t));
	data->len = id_len;
	data->dialogue_ids = malloc(sizeof(size_t) * id_len);
	strcpy(data->name, name);
	data->on_end.is_lua = 0;
	data->on_end.c = on_end;

	va_list args;
	va_start(args, id_len);
	for (size_t i = 0; i < id_len; i++)
		data->dialogue_ids[i] = va_arg(args, size_t);
	va_end(args);

	game_open_gui(data, on_dialogue_update, on_dialogue_render, on_dialogue_free);
}

void open_dialogue2(char *name, c_lua_obj_t on_end, size_t id_len, size_t *ids) {
	if (strlen(name) > 511)
		return ;

	dialogue_gui_info_t *data = calloc(1, sizeof(dialogue_gui_info_t));
	data->len = id_len;
	data->dialogue_ids = malloc(sizeof(size_t) * id_len);
	memcpy(data->dialogue_ids, ids, sizeof(size_t) * id_len);
	strcpy(data->name, name);
	data->on_end = on_end;

	game_open_gui(data, on_dialogue_update, on_dialogue_render, on_dialogue_free);
}
