#include "game.h"

int dialogue_register(char *id, char *val) {
	if (strlen(id) > 255) {
		PRINT_ERR("Error: dialog indentifier too long '%s' %s\n", id, game_lang);
		return 1;
	}
	dialogue_info_t *new_dialog = realloc(dialogue_infos, sizeof(dialogue_info_t) * (dialogue_infos_len + 1));
	if (!new_dialog) {
		perror("realloc");
		return 1;
	}
	dialogue_infos = new_dialog;
	dialogue_infos[dialogue_infos_len] = parse_dialogue(val);
	strcpy(dialogue_infos[dialogue_infos_len].id, id);
	if (!dialogue_infos[dialogue_infos_len].text)
		return 1;
	dialogue_infos_len++;
	return 0;
}

dialogue_info_t *get_dialogue_by_id(size_t id) {
	if (id >= dialogue_infos_len)
		id = 0;
	return &dialogue_infos[id];
}

size_t get_dialogue_id(const char *name) {
	for (size_t i = 0; i < dialogue_infos_len; i++) {
		if (!strcmp(dialogue_infos[i].id, name))
			return i;
	}
	return 0;
}
