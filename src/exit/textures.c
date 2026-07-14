#include "game.h"

void textures_free() {
	for (uint32_t i = 0; i < atlases_len; i++)
		SDL_DestroyTexture(atlases[i]);
	free(texture_registry);
	texture_registry = NULL;
	texture_registry_len = 0;
	atlases = NULL;
	atlases_len = 0;
}

