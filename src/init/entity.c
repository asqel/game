#include "game.h"

static entity_info_t	*entities_info = NULL;
static int				entities_len = 0;

entity_info_t *get_entity_info(uint32_t id) {
	if (id >= (uint32_t)entities_len)
		return NULL;
	return &entities_info[id];
}

void exit_entity() {
	free(entities_info);
	entities_info = NULL;
	entities_len = 0;
}
