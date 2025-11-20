#include "game.h"

static entity_info_t	*entities_info = NULL;
static int				entities_len = 0;

int init_entity() {
	return 0;
}

entity_info_t *get_entity_info(uint32_t id) {
	if (id >= (uint32_t)entities_len)
		return NULL;
	return &entities_info[id];
}

int register_entity(char *name, int sprite_id, double hx, double hy, double hw, double hh, double hp, int (*on_tick)(entity_t *self)) {
	if (strlen(name) > OBJ_NAME_LENGTH) {
		PRINT_ERR("Error: entity name `%s' too long\n", name);
		return 1;
	}
	for (int i = 0; i < entities_len; i++) {
		if (!strcmp(name, entities_info[i].name)) {
			PRINT_ERR("Error: entity %s already exists\n", name);
			return 1;
		}
	}
	entities_info = realloc(entities_info, sizeof(entity_info_t) * (entities_len + 1));

	strcpy(entities_info[entities_len].name, name);
	entities_info[entities_len].sprite_id = sprite_id;
	entities_info[entities_len].default_hitbox_x = hx;
	entities_info[entities_len].default_hitbox_y = hy;
	entities_info[entities_len].default_hitbox_w = hw;
	entities_info[entities_len].default_hitbox_h = hh;
	entities_info[entities_len].on_tick.c = on_tick;
	entities_info[entities_len].on_tick.is_lua = 0;
	entities_info[entities_len].hp = hp;
	return 0;
}

void exit_entity() {
	free(entities_info);
	entities_info = NULL;
	entities_len = 0;
}
