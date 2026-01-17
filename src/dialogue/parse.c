#include "game.h"

static uint8_t last_type = 0xff;

static void buff_append_text(dialogue_info_t *res, size_t idx, char c) {
	if (!res->text) {
		if (last_type != DIALOG_TEXT || c == '\\')
			res->len++;
		last_type = DIALOG_TEXT;
		return ;
	}
	if (last_type != DIALOG_TEXT || c == '\\') {
		res->text[res->len].type = DIALOG_TEXT;
		res->text[res->len].text[0] = idx;
		res->text[res->len].text[1] = 0;
		res->len++;
		last_type = DIALOG_TEXT;
	}
	res->text[res->len - 1].text[1]++;
}

static void buff_append(dialogue_info_t *res, dialogue_char_t *c) {
	if (res->text) {
		if (res->len > 0 && res->text[res->len - 1].type == DIALOG_TEXT) {
			res->text[res->len].type = 0xff;
			res->len++;
			
		}
		res->text[res->len] = *c;
	}
	else if (last_type == DIALOG_TEXT)
		res->len++;
	last_type = c->type;
	res->len++;
}

static size_t parse_sizet(dialogue_info_t *res, char *str, uint8_t type) {
	size_t val = 0;

	size_t i = 0;
	while (str[i] && isdigit(str[i])) {
		val = val * 10 + str[i] - '0';
		i++;
	}
	dialogue_char_t c = {0};
	c.type = type;
	c.var = val;
	buff_append(res, &c);
	return i;
}

static size_t parse_control(dialogue_info_t *res, char *str) {
		dialogue_char_t c = {0};	
	switch (str[0]) {
		case '$':
			return 1 + parse_sizet(res, str + 1, DIALOG_VAR);
		case 'P':
			return 1 + parse_sizet(res, str + 1, DIALOG_PAUSE);
		case 'E':
			return 1 + parse_sizet(res, str + 1, DIALOG_EMOTE);		
		case 'X':
			return 1 + parse_sizet(res, str + 1, DIALOG_PX);
		case 'Y':
			return 1 + parse_sizet(res, str + 1, DIALOG_PY);
		case 'R':
			return 1 + parse_sizet(res, str + 1, DIALOG_CR);
		case 'G':
			return 1 + parse_sizet(res, str + 1, DIALOG_CG);
		case 'B':
			return 1 + parse_sizet(res, str + 1, DIALOG_CB);
		case 'S':
			return 1 + parse_sizet(res, str + 1, DIALOG_STYLE);
		case 'F':
			return 1 + parse_sizet(res, str + 1, DIALOG_FONT);
		case 'W':
			c.type = DIALOG_WAIT;
			buff_append(res, &c);
			return 1;
		case 'n':
			c.type = DIALOG_LINE;
			buff_append(res, &c);
			return 1;
		default:
			c.type = 0xff;
			buff_append(res, &c);
			return 1;
	}
}

static void parse_buffer(dialogue_info_t *res, char *str) {
	last_type = 0xff;
	size_t i = 0;
	while (str[i]) {
		if (str[i] == '\\') {
			i++;
			if (!str[i]) {
				buff_append_text(res, i - 1, '\\');
				continue;
			}
			if (str[i] == '\\') {
				buff_append_text(res, i - 1, '\\');
				i++;
				continue;
			}
			i += parse_control(res, &str[i]);
			continue;
		}
		buff_append_text(res, i, str[i]);
		i++;
	}
}



dialogue_info_t parse_dialogue(char *str) {
	dialogue_info_t res = {0};

	parse_buffer(&res, str);

	res.text = malloc(sizeof(dialogue_char_t) * res.len);	
	res.original = strdup(str);

	if (!res.text || !res.original) {
		free(res.text);
		free(res.original);
		res.text = NULL;
		res.original = NULL;
		perror("malloc");
		return res;
	}

	res.len = 0;
	parse_buffer(&res, str);

	return res;
}