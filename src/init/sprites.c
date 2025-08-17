#include "game.h"

int init_sprites() {
	register_sprite((int []){game_texture_get_id("grass0")}, 1, 0, "grass", 0);
	register_sprite((int []){game_texture_get_id("tree")}, 1, 0, "tree", 0);
	register_sprite((int []){game_texture_get_id("red")}, 1, 0, "red", 0);
	return 0;
}
