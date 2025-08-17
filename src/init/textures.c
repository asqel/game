#include "game.h"

void init_texture_callback(const char *filepath) {
	if (strlen(filepath) < 4 || strcmp(filepath + strlen(filepath) - 3, ".tx") != 0)
		return;
	int path_len = strlen(filepath);
	char *png_path = malloc(path_len + 2);
	if (!png_path) {
		PRINT_ERR("Error: Memory allocation failed (init_texture_callback)\n");
		return;
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
}

int init_textures() {
	char *assets_path = malloc(strlen(game_dir) + 10);
	if (!assets_path) {
		PRINT_ERR("Error: Memory allocation failed (init_textures)\n");
		return 1;
	}
	sprintf(assets_path, "%s/assets", game_dir);

	game_list_files(assets_path, init_texture_callback);
	printf("texture number %d\n", texture_registry_len);
	free(assets_path);
	return 0;
}
