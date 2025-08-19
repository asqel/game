#include "game.h"

int game_action_pressed_hold(int action, int duration) {
	if (action <= 0 || action >= GAME_ACT_ENUM_MAX)
		return 0;
	if (game_ctx->actions[action] == 1)
		return 1;
	return game_ctx->actions[action] > duration;
	
}

int get_action_from_scancode(int scancode) {
	if (scancode == SDL_SCANCODE_W)
		return GAME_ACT_UP;
	if (scancode == SDL_SCANCODE_D)
		return GAME_ACT_RIGHT;
	if (scancode == SDL_SCANCODE_S)
		return GAME_ACT_DOWN;
	if (scancode == SDL_SCANCODE_A)
		return GAME_ACT_LEFT;
	if (scancode == SDL_SCANCODE_E)
		return GAME_ACT_INTERACT;
	if (scancode == SDL_SCANCODE_I)
		return GAME_ACT_EDIT_UP;
	if (scancode == SDL_SCANCODE_L)
		return GAME_ACT_EDIT_RIGHT;
	if (scancode == SDL_SCANCODE_K)
		return GAME_ACT_EDIT_DOWN;
	if (scancode == SDL_SCANCODE_J)
		return GAME_ACT_EDIT_LEFT;
	if (scancode == SDL_SCANCODE_P)
		return GAME_ACT_EDIT_PLACE;
	if (scancode == SDL_SCANCODE_O)
		return GAME_ACT_EDIT_REMOVE;
	if (scancode == SDL_SCANCODE_EQUALS)
		return GAME_ACT_EDIT_FILL;
	if (scancode == SDL_SCANCODE_MINUS)
		return GAME_ACT_EDIT_EMPTY;
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
		if (event.type == GAME_EVTY_PRESSED)
			game_ctx->actions[event.key.action]++;
		else if (event.type == GAME_EVTY_RELEASED)
			game_ctx->actions[event.key.action] = 0;
	}
}