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
		dialogue_exit();
		return 1;
	}
	free(text);
	return 0;
}

int dialogue_exit() {
	for (int i = 0; i < dialogue_infos_len; i++)
		free(dialogue_infos[i].text);
	free(dialogue_infos);
	dialogue_infos = NULL;
}
