#include "game.h"

entity_info_t *entities_infos = NULL;
int entities_infos_len = 0;

uint32_t entity_register(char *name, int sprite_id, c_lua_obj_t tick, double hp, int hit[4]) {
	if (strlen(name) > OBJ_NAME_LENGTH) {
		PRINT_ERR("Error: entity name too long %s\n", name);
		game_exit(1);
	}
	entities_infos_len++;
	entities_infos = realloc(entities_infos, sizeof(entity_info_t) * (entities_infos_len));

	entity_info_t *info = &entities_infos[entities_infos_len - 1];
	strcpy(info->name, name);
	info->sprite_id = sprite_id;
	info->default_hitbox_x = hit[0];
	info->default_hitbox_y = hit[1];
	info->default_hitbox_w = hit[2];
	info->default_hitbox_h = hit[3];
	info->on_tick = tick;
	info->hp = hp;
	return entities_infos_len - 1;
}

void entity_exit() {
	for (int i = 0; i < entities_infos_len; i++) {
		if (info->on_tick.is_lua)
			luaL_unref(lua_state, LUA_REGISTRYINDEX, info->on_tick.lua_ref);
	}
	free(entities_infos);
	entities_infos = NULL;
	entities_infos_len = 0;
}
