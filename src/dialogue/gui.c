#include "game.h"

typedef struct dialogue_gui_info_t {
	int *dialogue_ids;
	int len;
	int (*on_end)(gui_t *self); // returns 1 if no close
	char name[512]; // name above dialogue box

	int current_idx;
	int current_len;

	int sleep; // to time to sleep
	int wait; // do we wait for interact

	char *stash;

	uint32_t *to_print;
	int to_print_len;
	int r;
	int g;
	int b;
} dialogue_gui_info_t;

static void add_to_print(uint32_t c, dialogue_gui_info_t *infos) {
	infos->to_print_len++;
	infos->to_print = realloc(infos->to_print, sizeof(uint32_t) * infos->to_print_len);
	infos->to_print[infos->to_print_len - 1] = c;
}

void on_dialogue_free(gui_t *self) {
	dialogue_gui_info_t *data = self->data;
	free(data->stash);
	free(data->to_print);
	free(data->dialogue_ids);
	free(data);
}

void on_dialogue_render(gui_t *self) {
	dialogue_gui_info_t *data = self->data;
	if (!data->to_print)
		return ;
	int y = 30;
	int i = 0;
	while (i < data->to_print_len) {
		if (data->to_print[i] == (uint32_t)'\n') {
			i++;
			y += 20;
			continue;
		}
		int len = 0;
		int end = i;
		while (end < data->to_print_len && data->to_print[end] != '\n')
			end++;
		len = end - i;
		display_dialogue(&data->to_print[i], len, 100, y, data->r, data->g, data->b);
		i = end;
	}
}

void on_dialogue_update(gui_t *self) {
	dialogue_gui_info_t *data = self->data;
	if (data->sleep) {
		data->sleep--;
		return ;
	}
	if (data->stash) {
		add_to_print(data->stash[0], data);
		memmove(data->stash, &data->stash[1], strlen(data->stash));
		if (!data->stash[0]) {
			free(data->stash);
			data->stash = NULL;
		}
		return ;
	}
	if (!data->wait) {
		dialogue_info_t *dialogue = &dialogue_infos[data->current_idx];
		uint32_t c = dialogue->text[data->current_len] & 0x00FFFFFF;
		uint32_t type = dialogue->text[data->current_len] & 0xFF000000;
		if (type == DIALOG_CHAR) {
			if (c == 0) {
				data->current_idx++;
				data->current_len = 0;
				if (data->current_idx < data->len) {
					free(data->stash);
					free(data->to_print);
					data->stash = NULL;
					data->to_print = NULL;
					data->to_print_len = 0;
					return ;
				}
				if (!data->on_end || !data->on_end(self))
					game_close_gui();
				return ;
			}
			else if (c >> 7 == 0) {
				add_to_print(c, data);
				data->current_len++;
			}
			else if (c >> 4 == 0b1111) {
				add_to_print(c, data);
				add_to_print(dialogue->text[data->current_len + 1], data);
				add_to_print(dialogue->text[data->current_len + 2], data);
				add_to_print(dialogue->text[data->current_len + 3], data);
				data->current_len += 4;
			}
			else if (c >> 5 == 0b111) {
				add_to_print(c, data);
				add_to_print(dialogue->text[data->current_len + 1], data);
				add_to_print(dialogue->text[data->current_len + 2], data);
				data->current_len += 3;
			}
			else if (c >> 6 == 0b11) {
				add_to_print(c, data);
				add_to_print(dialogue->text[data->current_len + 1], data);
				data->current_len += 2;
			}
			else {
				add_to_print(c, data);
				data->current_len++;
			}
		}
		else if (type == DIALOG_PAUSE) {
			data->sleep = c;
			data->current_len++;
		}
		else if (type == DIALOG_WAIT) {
			data->current_len++;
			data->wait = 1;
		}
		else if (type == DIALOG_VAR) {
			data->stash = strdup("VAR");
			data->current_len++;
		}
		else if (type == DIALOG_EMOTE) {
			add_to_print(c | type, data);
			data->current_len++;
		}
		else {
			PRINT_ERR("ERROR: unknown dialogue type %d\n", type);
			game_exit(1);
		}
		return ;
	}
	if (game_ctx->actions[GAME_ACT_INTERACT])
		data->wait = 0;
}

void open_dialogue(char *name, int (*on_end)(gui_t *self), int id_len, ...) {
	if (strlen(name) > 511)
		return ;

	dialogue_gui_info_t *data = calloc(1, sizeof(dialogue_gui_info_t));
	data->len = id_len;
	data->dialogue_ids = malloc(sizeof(int) * id_len);
	strcpy(data->name, name);
	data->on_end = on_end;

	va_list args;
	va_start(args, id_len);
	for (int i = 0; i < id_len; i++)
		data->dialogue_ids[i] = va_arg(args, int);
	va_end(args);

	game_open_gui(data, on_dialogue_update, on_dialogue_render, on_dialogue_free);
}
