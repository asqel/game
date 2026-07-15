#include "game.h"

void *game_alloc(size_t size) {
	#undef malloc
	return malloc(size);
}
