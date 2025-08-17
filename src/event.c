#include "game.h"

int get_action_from_scancode(int scancode) {
	if (scancode == SDL_SCANCODE_W)
		return GAME_ACT_UP;
	if (scancode == SDL_SCANCODE_D)
		return GAME_ACT_RIGHT;
	if (scancode == SDL_SCANCODE_S)
		return GAME_ACT_DOWN;
	if (scancode == SDL_SCANCODE_A)
		return GAME_ACT_LEFT;
	return GAME_ACT_NONE;
}

game_event_t game_event_from_sdl(SDL_Event *ev) {
	game_event_t res = (game_event_t){0};

	if (ev->type == SDL_KEYDOWN || ev->type == SDL_KEYUP) {
		res.type = (ev->type == SDL_KEYDOWN) ? GAME_EVTY_PRESSED : GAME_EVTY_RELEASED;
		res.key.scancode = ev->key.keysym.scancode;
		res.key.action = get_action_from_scancode(res.key.scancode);
	}
	else if (ev->type == SDL_TEXTINPUT) {
		res.type = GAME_EVTY_TEXT;
		memcpy(res.text, ev->text.text, 32);
	}
	return res;
}

void game_enable_text_input() {
	SDL_StartTextInput();
}

void game_disable_text_input() {
	SDL_StopTextInput();
}

void update_event() {
	SDL_Event ev;
	while (SDL_PollEvent(&ev) !=  0) {
		if (ev.type == SDL_QUIT)
			game_exit(0);
		game_event_t event = game_event_from_sdl(&ev);
		if (event.type == GAME_EVTY_PRESSED && event.key.scancode == SDL_SCANCODE_E)
			player_interact();
		if (event.type == GAME_EVTY_PRESSED)
			game_ctx->actions[event.key.action]++;
		else if (event.type == GAME_EVTY_RELEASED)
			game_ctx->actions[event.key.action] = 0;
	}
}