#include "game.h"


world_t *game_new_world(char *name) {
	world_t *res = malloc(sizeof(world_t));

	strcpy(res->name, name);
	res->chunks = NULL;
	res->chunks = NULL;
	res->width = 0;
	res->height = 0;
	return res;
}

static void read_layer(int layer, chunk_t *chunk, FILE *f, uint8_t c_info) {
	printf("layer %d info %x res %d", layer, c_info, ((c_info >> (6 - layer)) & 0b1) == 0);
	if (((c_info >> (6 - layer)) & 0b1) == 0)
		return ;
	if (((c_info >> (3 - layer)) & 0b1) == 1) {
		uint32_t id = 0;
		fread(&id, 1, 3, f);
		obj_t o = game_get_obj(id);
		for (int i = 0; i < CHUNK_SIZE; i++) {
			for (int k = 0; k < CHUNK_SIZE; k++) {
				chunk->objs[i][k][layer] = o;	
			}
		}
		return ;
	}
	int x = 0;
	int y = 0;
	while (y < CHUNK_SIZE) {
		obj_t obj = (obj_t){0};
		uint8_t info;
		fread(&info, 1, sizeof(uint8_t), f);
		if (info >> 7)
			obj.id = 0;
		else
			fread(&obj.id, 1, 3, f);
		if (((info >> 6) & 0b1) == 1 && info >> 7 == 0)
			fread(&obj.data, 1, sizeof(uint32_t), f);
		int repeat = info & 0b111111;
		repeat++;
		obj = game_get_obj(obj.id);
		while (repeat--) {
			chunk->objs[y][x][layer] = obj;
			x++;
			if (x >= CHUNK_SIZE) {
				x = 0;
				y++;
			}
		}
	}


}

world_t *game_load_world(char *name) {
	if (strlen(name) > GAME_WORLD_NAME_MAX_LEN)
		return NULL;
	char *world_path = game_get_world_path(name);

	FILE *f = fopen(world_path, "rb");
	if (f == NULL) {
		free(world_path);
		return game_new_world(name);
	}
	world_t *res = malloc(sizeof(world_t));
	res->height = 0;
	res->width = 0;
	res->chunks = NULL;
	strcpy(res->name, name);
	fread(&res->width, 1, sizeof(int32_t), f);
	fread(&res->height, 1, sizeof(int32_t), f);
	res->chunks = malloc(sizeof(chunk_t **) * res->height);
	for (int i = 0; i < res->height; i++)
		res->chunks[i] = calloc(res->width, sizeof(chunk_t *));
	for (int i = 0; i < res->height; i++) {
		for (int k = 0; k < res->width; k++) {
			uint8_t c_info = 0;
			fread(&c_info, 1, 1, f);
			if (c_info >> 7 == 1)
				continue;
			res->chunks[i][k] = calloc(1, sizeof(chunk_t));
			read_layer(0, res->chunks[i][k], f, c_info);
			read_layer(1, res->chunks[i][k], f, c_info);
			read_layer(2, res->chunks[i][k], f, c_info);
		}
	}
	printf("TES GAND PARENT\n");
	return res;	
}

chunk_t *world_get_chunk(world_t *world, int cx, int cy) {
	if (cx < 0 || cy < 0)
		return NULL;
	if (cx < world->width && cy < world->height) {
		if (world->chunks[cy][cx] == NULL)
			world->chunks[cy][cx] = game_load_chunk(world, cx, cy);
		return world->chunks[cy][cx];
	}
	if (cy >= world->height) {
		world->chunks = realloc(world->chunks, sizeof(chunk_t **) * (cy + 1))	;
		for (int i = world->height; i <= cy; i++) {
			world->chunks[i] = malloc(sizeof(chunk_t *) * world->width);
			for (int k = 0; k < world->width; k++)
				world->chunks[i][k] = NULL;
		}
		world->height = cy + 1;
		return world_get_chunk(world, cx, cy);
	}
	if (cx >= world->width) {
		for (int i = 0; i < world->height; i++) {
			world->chunks[i] = realloc(world->chunks[i], sizeof(chunk_t *) * (cx + 1));
			for (int k = world->width; k <= cx; k++)
				world->chunks[i][k] = NULL;
		}
		world->width = cx + 1;
	}
	return world_get_chunk(world, cx, cy);
}