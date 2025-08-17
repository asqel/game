#include "game.h"

int init_objects() {
	game_register_obj("air", 0, 0, NULL);
	game_register_obj("grass", get_sprite_id("grass"), 0, NULL);
	game_register_obj("tree", get_sprite_id("tree"), 1, NULL);
	return 0;
}
