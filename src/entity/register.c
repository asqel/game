#include "game.h"

entity_info_t *entities_infos = NULL;
int entities_infos_len = 0;

uint32_t entity_register(char *name, int sprite_id, int tick_ref, double hp, double world_hit[4], double drag) {
	if (strlen(name) > OBJ_NAME_LENGTH) {
		PRINT_ERR("Error: entity name too long %s\n", name);
		game_exit(1);
	}
	entities_infos_len++;
	entities_infos = realloc(entities_infos, sizeof(entity_info_t) * (entities_infos_len));

	entity_info_t *info = &entities_infos[entities_infos_len - 1];
	strcpy(info->name, name);
	info->sprite_id = sprite_id;
	info->default_world_hitbox_x = world_hit[0];
	info->default_world_hitbox_y = world_hit[1];
	info->default_world_hitbox_w = world_hit[2];
	info->default_world_hitbox_h = world_hit[3];
	info->on_tick_ref = tick_ref;
	info->hp = hp;
	info->drag = drag;
	return entities_infos_len - 1;
}

void entity_exit() {
	for (int i = 0; i < entities_infos_len; i++) {
		entity_info_t *info = &entities_infos[i];
		luaL_unref(lua_state, LUA_REGISTRYINDEX, info->on_tick_ref);
	}
	free(entities_infos);
	entities_infos = NULL;
	entities_infos_len = 0;
}
