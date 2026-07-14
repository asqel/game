#include "game.h"

void init_texture_callback(const char *filepath) {
	if (strlen(filepath) < 4 || strcmp(filepath + strlen(filepath) - 3, ".tx") != 0)
		return;

	size_t path_len = strlen(filepath);
	char *png_path = malloc(path_len + 2);
	if (!png_path) {
		PRINT_ERR("Error: Memory allocation failed (init_texture_callback)\n");
		exit(1);
	}

	strcpy(png_path, filepath);
	png_path[path_len - 2] = 'p';
	png_path[path_len - 1] = 'n';
	png_path[path_len] = 'g';
	png_path[path_len + 1] = '\0';
	if (!game_path_is_file(png_path)) {
		free(png_path);
		return;
	}
	game_load_tx(png_path, filepath);
	free(png_path);
}

SDL_Texture *missing_atlas(uint32_t color1, uint32_t color2, uint32_t color3, uint32_t color4) {
	SDL_Texture *res = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, 4, 2);
	if (!res) {
		PRINT_ERR("Error: Memory allocation failed (%s)\n", __func__);
		exit(1);
	}

	uint32_t pixels[8] = {
		color1, color2, color3, color4,
		color2, color1, color4, color3
	};

	SDL_UpdateTexture(res, NULL, pixels, 4 * sizeof(uint32_t));
	return res;
}

void init_textures() {
	char *assets_path = malloc(strlen(game_dir) + 10);
	if (!assets_path) {
		PRINT_ERR("Error: Memory allocation failed (%s)\n", __func__);
		exit(1);
	}
	sprintf(assets_path, "%s/assets", game_dir);

	add_atlas(missing_atlas(0xFF00FFFF, 0x000000FF, 0x00FF00FF, 0xFFFFFFFF));
	add_texture("", &(SDL_Rect){.x = 0, .y = 0, .w = 2, .h = 2}, TILE_SIZE, TILE_SIZE, 0, "None");
	add_texture(" ", &(SDL_Rect){.x = 2, .y = 0, .w = 2, .h = 2}, TILE_SIZE, TILE_SIZE, 0, "None");

	game_list_files(assets_path, init_texture_callback);
	free(assets_path);
}

uint32_t add_atlas(SDL_Texture *tx) {
	void *tmp = realloc(atlases, sizeof(SDL_Texture *) * (atlases_len + 1));
	if (!tmp) {
		PRINT_ERR("Error: Memory allocation faild (%s)\n", __func__);
		exit(1);
	}

	atlases = tmp;
	atlases[atlases_len] = tx;
	atlases_len++;
	return atlases_len - 1;
}

void add_texture(const char *name, SDL_Rect *src_rect, int dest_w, int dest_h, uint32_t atlas, const char *path) {
	if (strlen(name) > TEXTURE_NAME_LENGTH) {
		PRINT_ERR("Error: Texture name too long skipping %s: %s\n", name, path);
		return ;
	}

	if (atlas >= atlases_len) {
		PRINT_ERR("Error: Texture %s needs %u but id does not exists: %s\n", name, atlas, path);
		return ;
	}

	for (uint32_t i = 0; i < texture_registry_len; i++) {
		if (strcmp(texture_registry[i].name, name) == 0) {
			PRINT_ERR("Error: Texture defined a second time %s: %s\n", name, path);
			return ;
		}
	}

	void *tmp = realloc(texture_registry, sizeof(texture_t) * (texture_registry_len + 1));
	if (!tmp) {
		PRINT_ERR("Error: Memory allocation faild (%s)\n", __func__);
		exit(1);
	}
	texture_registry = tmp;

	texture_registry[texture_registry_len].src_rect = *src_rect;
	texture_registry[texture_registry_len].dest_w = dest_w;
	texture_registry[texture_registry_len].dest_h = dest_h;
	texture_registry[texture_registry_len].atlas_idx = atlas;
	strcpy(texture_registry[texture_registry_len].name, name);

	texture_registry_len++;
}
