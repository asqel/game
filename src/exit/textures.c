#include "game.h"

void textures_free() {
	for (uint32_t i = 0; i < texture_registry_len; i++)
		SDL_FreeSurface(texture_registry[i].surface);
	free(texture_registry);
	texture_registry = NULL;
	texture_registry_len = 0;
}

