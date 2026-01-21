#include "game.h"

int lua_func_draw_text(lua_State *l) {
	if (lua_gettop(l) != 1) {
		lua_pushstring(l, "Error: draw_text takes only 1 argument\n");
		return lua_error(l);
	}
	if (lua_type(l, 1) != LUA_TSTRING) {
		lua_pushstring(l, "Error: draw_text only takes a string as argument\n");
		return lua_error(l);
	}
	size_t dialogue_id = get_dialogue_id(lua_tostring(l, 1));
	dialogue_info_t *dialogue = get_dialogue_by_id(dialogue_id);
	int x = 0;
	int y = 0;
	size_t font = 0;
	for (size_t i = 0; i < dialogue->len; i++) {
		if (dialogue->text[i].type == DIALOG_PX)
			x = dialogue->text[i].px;
		else if (dialogue->text[i].type == DIALOG_PY)
			y = dialogue->text[i].py;
		else if (dialogue->text[i].type == DIALOG_FONT)
			font = dialogue->text[i].font;
		else
			break;
	}
	display_dialogue(dialogue, dialogue->len, 0, 0, 0, 0, x, y, font);
	return 0;
}
