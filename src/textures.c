#include "game.h"

static void treat_tx(char *tx_path, char *text, uint32_t atlas) {
	int p = 0;
	while (text[p]) {
		if (strchr("\n\r", text[p]) || isblank(text[p])) {
			p++;
			continue;
		}
		if (text[p] == '-') {
			while (text[p] && !strchr("\n\r", text[p]))
				p++;
			continue;
		}

		int tx_info[6] = {-1, -1 , -1, -1, -1, -1};
		int tx_info_end = 0;
		int sep_pos = p;

		while (text[sep_pos] && !strchr("$\n\r", text[sep_pos]))
			sep_pos++;

		if (strchr("\n\r", text[sep_pos])) {
			p = sep_pos;
			continue;
		}
			
		if (text[sep_pos] != '$') {
			PRINT_ERR("ERROR: missing separator '$', skipping file: %s\n", tx_path);
			return ;
		}

		for (int i = p; i < sep_pos; i++) {
			if (!isblank(text[i]) && !isdigit(text[i])) {
				PRINT_ERR("ERROR: only ' ' or [0-9] before '$', skipping file: %s\n", tx_path);
				return ;
			}
		}
		while (p < sep_pos) {
			if (isblank(text[p]) && (p - 1 >= 0 && isdigit(text[p - 1]))) {
				p++;
				tx_info_end++;
				continue;
			}

			if (isdigit(text[p])) {
				if (tx_info[tx_info_end] == -1)
					tx_info[tx_info_end] = 0;
				tx_info[tx_info_end] *= 10;
				tx_info[tx_info_end] += text[p] - '0';
				p++;
				continue;
			}
			p++;
		}
		for (int i = 0; i < 6; i++) {
			if (tx_info[i] == -1) {
				PRINT_ERR("ERROR: Expected 6 number before '$', skipping file : %s\n", tx_path);
				return ;
			}
		}

		int name_start = sep_pos + 1;
		int name_end = name_start;

		while (text[name_end] && !strchr("\n\r", text[name_end]))
			name_end++;

		while (isblank(text[name_start]))
			name_start++;
		while (isblank(text[name_end - 1]))
			name_end--;

		if (name_end <= name_start) {
			PRINT_ERR("ERROR: Expected name after '$', skipping file: %s\n", tx_path);
			return ;
		}

		char c = text[name_end];
		text[name_end] = '\0';

		SDL_Rect rect = {0};
		rect.x = tx_info[0];
		rect.y = tx_info[1];
		rect.w = tx_info[2];
		rect.h = tx_info[3];
		add_texture(&text[name_start], &rect, tx_info[4], tx_info[5], atlas, tx_path);

		text[name_end] = c;
		p = name_end;
	}
}

void game_load_tx(const char *png_path, const char *tx_path) {
	SDL_Texture *atlas = IMG_LoadTexture(renderer, png_path);
	if (!atlas) {
		PRINT_ERR("Error: Failed to load texture '%s': %s\n", png_path, IMG_GetError());
		return;
	}
	FILE *tx_file = fopen(tx_path, "rb");
	if (!tx_file) {
		PRINT_ERR("Error: Failed to open texture file '%s': %s\n", tx_path, strerror(errno));
		SDL_DestroyTexture(atlas);
		return;
	}
	char *tx_content = NULL;
	int tx_size = 0;

	fseek(tx_file, 0, SEEK_END);
	tx_size = ftell(tx_file);
	fseek(tx_file, 0, SEEK_SET);

	tx_content = malloc(tx_size + 1);
	if (!tx_content) {
		PRINT_ERR("Error: Memory allocation failed (%s)\n", __func__);
		return;
	}

	fread(tx_content, 1, tx_size, tx_file);
	tx_content[tx_size] = '\0';

	treat_tx((char *)tx_path, tx_content, add_atlas(atlas));

	free(tx_content);
	fclose(tx_file);
}

uint32_t game_texture_get_id(const char *name) {
	for (uint32_t i = 0; i < texture_registry_len; i++) {
		if (strcmp(texture_registry[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}
