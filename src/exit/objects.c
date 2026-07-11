#include "game.h"

void objects_free() {
	free(obj_registry);
	obj_registry = NULL;
	obj_registry_len = 0;
}
