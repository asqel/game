#include "game.h"

static chunk_t *game_load_chunk(world_t *world, int x, int y) {
	(void)x;
	(void)y;
	(void)world;
	chunk_t *res = malloc(sizeof(chunk_t));
	for (int i = 0; i < 20; i++) {
		for (int k = 0; k < 20; k++) {
			res->objs[i][k][0].id = 0;
			res->objs[i][k][1].id = 0;
			res->objs[i][k][2].id = 0;
			res->hitbox[i][k] = 0;	
		}
	}
	return res;
}

chunk_t *world_new_chunk(int x, int y, world_t *world) {
	chunk_t *old = get_chunk(x, y, world);	
	if (old) {
		for (int i = 0; i < old->entities_len; i++) {
			c_lua_obj_t *data = &old->entities[i].data;		
			if (data->is_lua)
				luaL_unref(lua_state, LUA_REGISTRYINDEX, data->lua_ref);
			else if (data->c)
				free(data->c);
		}
		free(old->entities);
		bzero(old, sizeof(chunk_t));
		return old;
	}
	while (x < 0)
		x += world->width;
	while (x >= world->width)
		x -= world->width;
	while (y < 0)
		y += world->height;
	while (y >= world->height)
		y -= world->height;
	world->chunks[y][x] = game_load_chunk(world, x, y);
	return world->chunks[y][x];
}

chunk_t *world_new_chunk_at(int x, int y, world_t *world) {
	chunk_t *old = get_chunk_at(x, y, world);	
	if (old) {
		for (int i = 0; i < old->entities_len; i++) {
			c_lua_obj_t *data = &old->entities[i].data;		
			if (data->is_lua)
				luaL_unref(lua_state, LUA_REGISTRYINDEX, data->lua_ref);
			else if (data->c)
				free(data->c);
		}
		free(old->entities);
		bzero(old, sizeof(chunk_t));
		return old;
	}
	while (x < 0)
		x += world->width * CHUNK_SIZE;
	while (x >= world->width)
		x -= world->width * CHUNK_SIZE;
	while (y < 0)
		y += world->height * CHUNK_SIZE;
	while (y >= world->height)
		y -= world->height * CHUNK_SIZE;
	x /= CHUNK_SIZE;
	y /= CHUNK_SIZE;
	world->chunks[y][x] = game_load_chunk(world, x, y);
	return world->chunks[y][x];
}

chunk_t *get_chunk(int x, int y, world_t *world) {
	while (x < 0)
		x += world->width;
	while (x >= world->width)
		x -= world->width;
	while (y < 0)
		y += world->height;
	while (y >= world->height)
		y -= world->height;
	return world->chunks[y][x];
}

chunk_t *get_chunk_at(int x, int y, world_t *world) {
	while (x < 0)
		x += world->width * CHUNK_SIZE;
	while (x >= world->width * CHUNK_SIZE)
		x -= world->width * CHUNK_SIZE; 
	while (y < 0)
		y += world->height * CHUNK_SIZE;
	while (y >= world->height * CHUNK_SIZE)
		y -= world->height * CHUNK_SIZE;
	printf("%d %d\n", x, y);
	return world->chunks[y / CHUNK_SIZE][x / CHUNK_SIZE];
}
