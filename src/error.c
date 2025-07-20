#include "game.h"

char *game_get_error() {
	if (game_error_is_special) {
		// TODO
	}
	else if (game_error_str) {
		return game_error_str;
	}
	return "No error";
}

void game_set_error(const char *error) {
	if (game_error_str) {
		free(game_error_str);
	}
	game_error_str = strdup(error);
	game_error_is_special = 0;
}

void game_set_error_special(int error_code) {
	game_error_is_special = error_code;
	if (game_error_str) {
		free(game_error_str);
	}
	game_error_str = NULL;
}
