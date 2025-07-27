#include "game.h"

void sprite_free() {
	for (int i = 0; i < sprite_registry_len; i++)
		free(sprite_registry[i].textures_ids);
	free(sprite_registry);
	sprite_registry = NULL;
	sprite_registry_len = 0;
}
