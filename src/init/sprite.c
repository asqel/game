#include "game.h"

void init_sprite() {
	uint32_t textures[] = {0, 1};
	register_sprite((uint32_t *)textures, 2, GAME_FPS / 2, "", SPRITE_MASK_LOOP);
}
