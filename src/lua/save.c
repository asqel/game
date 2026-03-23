#include "game.h"

static char *read_file_bin(char *path, int *len) {
	FILE *f = fopen(path, "rb");
	if (!f)
		return NULL;
	fseek(f, 0, SEEK_END);
	*len = ftell(f);
	if (!*len) {
		fclose(f);
		return NULL;
	}
	char *res = malloc(*len);
	if (!res) {
		fclose(f);
		return NULL;
	}
	if (fread(res, 1, *len, f) != (size_t)*len) {
		free(res);
		fclose(f);
		return NULL;
	}
	return res;
}

int lua_func_read_save(lua_State *l) {
	if (lua_gettop(l) != 1 || lua_type(l, 1) != LUA_TSTRING) {
		lua_pushstring(l, "Error: read_save only takes one string as argument\n");
		return lua_error(l);
	}
	const char *name = lua_tostring(l, 1);
	for (int i = 0; name[i]; i++) {
		char c = name[i];
		if (!('a' <= c && c <= 'z') && !('A' <= c && c <= 'Z') && !strchr("0123456789-_", c)) {
			lua_pushstring(l, "Error: read_save: save name can only contain [a-zA-Z0-9-_]\n");
			return lua_error(l);
		}
	}

	int path_len = strlen(game_dir) + 1 + strlen("save") + 1 + strlen(name);
	char *path = malloc(path_len + 1);
	sprintf(path, "%s/save/%s", game_dir, name);
	int file_len = 0;
	char *file_content = read_file_bin(path, &file_len);
	if (!file_content)
		lua_pushnil(l);
	else
		lua_pushlstring(l, file_content, file_len);
	free(file_content);
	free(path);
	return 1;
}
