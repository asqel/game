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
	strcpy(dialogue_infos[dialogue_infos_len].id, id);
	dialogue_infos[dialogue_infos_len].text = parse_dialogue(val);
	if (!dialogue_infos[dialogue_infos_len].text)
		return 1;
	dialogue_infos_len++;
	return 0;
}
