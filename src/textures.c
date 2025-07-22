#include "game.h"

int texture_add(SDL_Surface *surface, const char *name) {
	if (strlen(name) > TEXTURE_NAME_LENGTH) {
		PRINT_ERR("Error: Texture name too long: '%s'\n", name);
		return -1;
	}
	void *tmp;
	for (int i = 0; i < texture_registry_len; i++) {
		if (strcmp(texture_registry[i].name, name) == 0) {
			PRINT_ERR("Error: Texture already exists: '%s'\n", name);
			return i;
		}
	}
	tmp = realloc(texture_registry, sizeof(texture_t) * (texture_registry_len + 1));
	if (!tmp) {
		PRINT_ERR("Error: Memory allocation failed (texture_add)\n");
		game_exit(1);
	}
	texture_registry = tmp;
	texture_registry[texture_registry_len].surface = surface;
	strcpy(texture_registry[texture_registry_len].name, name);
	texture_registry_len++;
	return 0;
}

static void add_image_cut(int *info, const char *name, SDL_Surface *surface) {
	SDL_Rect srcRect = {info[0], info[1], info[2], info[3]};
	SDL_Surface *before_resize = SDL_CreateRGBSurface(0, info[2], info[3], game_surface->format->BitsPerPixel,
			game_surface->format->Rmask, game_surface->format->Gmask, game_surface->format->Bmask, game_surface->format->Amask);
	if (before_resize == NULL) {
		PRINT_ERR("ERROR: could not allocate memory for image cutting\n%s\n", SDL_GetError());
	}

	SDL_BlitSurface(surface, &srcRect, before_resize, NULL);

	SDL_Surface *resized_image = SDL_CreateRGBSurface(0, info[4], info[5], game_surface->format->BitsPerPixel,
			game_surface->format->Rmask, game_surface->format->Gmask, game_surface->format->Bmask, game_surface->format->Amask);
	if (resized_image == NULL) {
		PRINT_ERR("ERROR: could not allocate memory for resizing\n");
		SDL_FreeSurface(before_resize);
		exit(1);
	}
	for (int y = 0; y < info[5]; y++) {
		for (int x = 0; x < info[4]; x++) {
			int srcX = x * info[2] / info[4];
			int srcY = y * info[3] / info[5];

			Uint32 pixel = ((Uint32*)before_resize->pixels)[srcY * before_resize->w + srcX];

			((Uint32*)resized_image->pixels)[y * resized_image->w + x] = pixel;
		}
	}

    // Libérer la mémoire
    SDL_FreeSurface(before_resize);
	texture_add(resized_image, name);
}

static void treat_tx(const char *tx_path, SDL_Surface *surface, const char *tx_content) {
	int p = 0;
	while (tx_content[p] != '\0') {
		if (tx_content[p] == '\n' || tx_content[p] == '\r') {
			p++;
			continue;
		}
		if (isblank(tx_content[p])) {
			p++;
			continue;
		}
		if (tx_content[p] == '-') {
			while (tx_content[p] != '\n' && tx_content[p] != '\r' && tx_content[p] != '\0')
				p++;
			continue;
		}

		int tx_info[6] = {-1, -1 , -1, -1, -1, -1};
		int tx_info_end = 0;
		int sep_pos = p;

		while (tx_content[sep_pos] != '\0' && tx_content[sep_pos] != '$' && tx_content[sep_pos] != '\n' && tx_content[sep_pos] != '\r')
			sep_pos++;
		if (tx_content[sep_pos] != '$') {
			PRINT_ERR("ERROR: missing separator '$' in file:\n\t%s\n", tx_path);
			exit(1);
		}
		for (int i = p; i < sep_pos; i++) {
			if (!isblank(tx_content[i]) && !('0' <= tx_content[i] && tx_content[i] <= '9')) {
				PRINT_ERR("ERROR: only spaces or digits (0-9) allowed on the left side of separator ($) in file:\n\t%s\n", tx_path);
				exit(1);
			}
		}
		while (p < sep_pos) {
			if (isblank(tx_content[p]) && (p - 1 >= 0 && ('0' <= tx_content[p - 1] && tx_content[p - 1] <= '9'))) {
				p++;
				tx_info_end++;
				continue;
			}
			if ('0' <= tx_content[p] && tx_content[p] <= '9') {
				if (tx_info[tx_info_end] == -1)
					tx_info[tx_info_end] = 0;
				tx_info[tx_info_end] *= 10;
				tx_info[tx_info_end] += tx_content[p] - '0';
				p++;
				continue;
			}
			p++;
		}
		for (int i = 0; i < 6; i++) {
			if (tx_info[i] == -1) {
				PRINT_ERR("ERROR: missing information (expected 6 number)\n\t%s\n", tx_path);
				exit(1);
			}
		}

		int name_start = sep_pos + 1;
		int name_end = name_start;

		while (tx_content[name_end] != '\0' && tx_content[name_end] != '\n' && tx_content[name_end] != '\r')
			name_end++;
		while (isblank(tx_content[name_start]))
			name_start++;
		while (isblank(tx_content[name_end - 1]))
			name_end--;
		if (name_end <= name_start) {
			PRINT_ERR("ERROR: missing information (expected a name)\n\t%s\n", tx_path);
			exit(1);
		}
		char *name = malloc(sizeof(char) * (name_end - name_start + 1));

		name[name_end - name_start] = '\0';
		for (int i = name_start; i < name_end; i++)
			name[i - name_start] = tx_content[i];
		add_image_cut(tx_info, name, surface);
		free(name);
		p = name_end;
	}
}

void game_load_tx(const char *png_path, const char *tx_path) {
	SDL_Surface *surface = IMG_Load(png_path);
	if (!surface) {
		PRINT_ERR("Error: Failed to load texture '%s': %s\n", png_path, IMG_GetError());
		return;
	}
	FILE *tx_file = fopen(tx_path, "rb");
	if (!tx_file) {
		PRINT_ERR("Error: Failed to open texture file '%s': %s\n", tx_path, strerror(errno));
		SDL_FreeSurface(surface);
		return;
	}
	char *tx_content = NULL;
	int tx_size = 0;
	fseek(tx_file, 0, SEEK_END);
	tx_size = ftell(tx_file);
	fseek(tx_file, 0, SEEK_SET);
	tx_content = malloc(tx_size + 1);
	if (!tx_content) {
		PRINT_ERR("Error: Memory allocation failed (game_load_tx)\n");
		SDL_FreeSurface(surface);
		fclose(tx_file);
		return;
	}
	tx_content[tx_size] = '\0';
	fread(tx_content, 1, tx_size, tx_file);
	fclose(tx_file);

	treat_tx(tx_path, surface, tx_content);

	free(tx_content);
	SDL_FreeSurface(surface);
}

int game_texture_get_id(const char *name) {
	for (int i = 0; i < texture_registry_len; i++) {
		if (strcmp(texture_registry[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}
