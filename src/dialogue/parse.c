#include "game.h"

static void buff_append_c(uint32_t **buffer, uint32_t c, int do_write) {
	if (do_write)
		*(*buffer) = c;
	(*buffer)++;
}

static int parse_number(unsigned char *str, uint32_t **buffer, uint32_t type, int do_write) {
	int i = 0;

	if (!isdigit(str[i])) {
		buff_append_c(buffer, '$', do_write);
		return 1;
	}
	uint32_t nb = 0;
	while (isdigit(str[i])) {
		nb = nb * 10 + str[i] - '0';
		i++;
	}
	if (str[i] == '.')
		i++;
	nb &= 0x00FFFFFF;
	nb |= type;
	buff_append_c(buffer, nb, do_write);
	return i;
}

static int parse_sequence(unsigned char *str, uint32_t **buffer, int do_write) {
	switch (*str) {
		case '$':
			return 1 + parse_number(str + 1, buffer, DIALOG_VAR, do_write);
		case 'P':
			return 1 + parse_number(str + 1, buffer, DIALOG_PAUSE, do_write);
		case 'E':
			return 1 + parse_number(str + 1, buffer, DIALOG_EMOTE, do_write);
		case 'X':
			return 1 + parse_number(str + 1, buffer, DIALOG_PX, do_write);
		case 'Y':
			return 1 + parse_number(str + 1, buffer, DIALOG_PY, do_write);
		case 'R':
			return 1 + parse_number(str + 1, buffer, DIALOG_CR, do_write);
		case 'G':
			return 1 + parse_number(str + 1, buffer, DIALOG_CG, do_write);
		case 'B':
			return 1 + parse_number(str + 1, buffer, DIALOG_CB, do_write);
		case 'S':
			return 1 + parse_number(str + 1, buffer, DIALOG_STYLE, do_write);
		case 'n':
			buff_append_c(buffer, '\n', do_write);
			return 1;
		case 'W':
			buff_append_c(buffer, DIALOG_WAIT, do_write);
			return 1;
		default:
			break;
	}
	buff_append_c(buffer, *str, do_write);
	return 1;
}


static int parse_buffer(uint32_t *buffer, unsigned char *str) {
	int i = 0;
	uint32_t *buffer_start = buffer;
	int do_write = buffer != NULL;
	while (str[i]) {
		if (str[i] == '\\') {
			i++;
			if (!str[i]) {
				buff_append_c(&buffer, '\\', do_write);
				continue;
			}
			i += parse_sequence(&str[i], &buffer, do_write);
			continue;
		}
		buff_append_c(&buffer, (uint32_t)str[i], do_write);
		i++;
	}
	return buffer - buffer_start;
}

uint32_t *parse_dialogue(char *str) {
	int res_len = parse_buffer(NULL, (unsigned char *)str);
	uint32_t *res = calloc(res_len + 1, sizeof(uint32_t));
	if (!res) {
		perror("malloc");
		return res;
	}
	parse_buffer(res, (unsigned char *)str);
	res[res_len] = 0;
	return res;
}