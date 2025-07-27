#include "game.h"

game_event_t game_event_from_sdl(SDL_Event *ev) {
	game_event_t res = (game_event_t){0};

	if (ev->type == SDL_KEYDOWN || ev->type == SDL_KEYUP) {
		res.type = (ev->type == SDL_KEYDOWN) ? GAME_EVTY_PRESSED : GAME_EVTY_RELEASED;
		res.key.scancode = ev->key.keysym.scancode;
		res.key.action = 0; //  TODO!
	}
	else if (ev->type == SDL_TEXTINPUT) {
		res.type = GAME_EVTY_TEXT;
		memcpy(res.text, ev->text.text, 32);
	}
	return res;
}