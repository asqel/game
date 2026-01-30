#include "game.h"

void game_free_dialogues() {
	for (size_t i = 0; i < dialogue_infos_len; i++) {
		free(dialogue_infos[i].original);	
		free(dialogue_infos[i].text);
	}
	free(dialogue_infos);
	dialogue_infos = NULL;
	dialogue_infos_len = 0;
}
