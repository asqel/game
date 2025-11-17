#include "game.h"

void test(int x, int y) {
	open_dialogue("AA", NULL, 2, (int []){0, 1});
}

int init_objects() {
	game_register_obj("air", 0, 0, NULL);
	game_register_obj("grass", get_sprite_id("grass"), 0, NULL);
	game_register_obj("tree", get_sprite_id("tree"), 0, NULL);
	//game_register_obj("player_house", get_sprite_id("player_house"), 1, NULL);
	game_register_obj_w_hit("player_house", get_sprite_id("player_house"), NULL, -1, 0, 1, 1);
	//game_register_obj_w_hit("fridge", get_sprite_id("fridge"), test, 0, 0, 0.4, 0.4);
	//game_register_obj("fridge", get_sprite_id("fridge"), 1, test);
	return 0;
}
