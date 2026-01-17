#include "game.h"

void objects_free() {
	for (size_t i = 0; i < obj_registry_len; i++) {
		if (obj_registry[i].interact.is_lua)
			luaL_unref(lua_state, LUA_REGISTRYINDEX, obj_registry[i].interact.lua_ref);
	}
	free(obj_registry);
	obj_registry = NULL;
	obj_registry_len = 0;
}
