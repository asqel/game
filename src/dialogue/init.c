#include "game.h"

dialogue_info_t		*dialogue_infos = NULL;
int					dialogue_infos_len = 0;
char				game_lang[6] = "en_uk";

char *get_lang_path() {
	int len = strlen(game_dir) + 1 + 4 + 1 + 6;
	char *res = malloc(len);
	if (!res)
		return res;
	sprintf(res, "%s/lang/%s", game_dir, game_lang);
	return res;
}

static int is_id_valid(char c) {
	if ('A' <= c && c <= 'Z')
		return 1;
	if ('a' <= c && c <= 'z')
		return 1;
	return strchr("0123456789-_+");
}

static int parse_line(char *text, int line) {
	int id_start = -1;
	int id_len = 0;
	int val_start = -1;
	int val_len = 0;

	int i = 0;
	while (isspace(text[i]) && text[i])
		i++;
	if (!is_id_valid(text[i]))
		return 1;
	id_start = i;
	id_len = 1;
	while (is_id_valid[i]) {
		i++;
		id_len++;
	}
	while (isspace(text[i]) && text[i])
		i++;
	if (text[i] != ':')
		return 1;
	i++;
	while (isspace(text[i]) && text[i])
		i++;
	if (text[i] != '"')
		return 1;
	i++;
	if (!text[i])
		return 1;
	val_start = i;
	val_len = 0;
	while(text[i] && text[i] != '"') {
		if (text[i] == '\\' && text[i + 1] == '"') {
			i++;
			continue;
		}
		val_len++;
		i++;
	}
	if (text[i] != '"')
		return 1;
	
	char *value = malloc(val_len + 1);
	char *id = malloc(id_len + 1);
	if (!id || !value) {
		free(value);
		free(id);
		return 1;
	}
	value[val_len] = '\0';
	id[id_len] = '\0';
	memcpy(&text[id_start], id, id_len);
	memcpy(&text[val_start], value, val_len);
	int ret =dialogue_register(value, id);
	free(id);
	free(value);
	return ret;
}

static int parse_lang(char *text) {
	int i = 0;
	int line = 1;
	while (text[i]) {
		if (text[i] == '\n') {
			i++;
			line++;
			continue;
		}	
		if (parse_line(&text[i], line)) {
		}
		i += line_len;
	}
}

int dialogue_init() {
	char *path = get_lang_path();
	if (!path) {
		perror("malloc");
		return 1;
	}
	char *text = read_file(path);
	if (!text) {
		printf("Error: could not read the lang file for %s\n", game_lang);
		free(path);
		return 1;
	}
	if (parse_lang(text)) {
		free(text);
		return 1;
	}
	free(text);
	return 0;
}
