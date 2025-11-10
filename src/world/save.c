#include "game.h"

static int is_layer_empty(chunk_t *chunk, int layer) {
	if (chunk == NULL)
		return 1;
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			if (chunk->objs[i][k][layer].id != 0)
				return 0;
		}
	}
	return 1;
}

static int is_layer_repeat(chunk_t *chunk, int layer) {
	int obj_id = chunk->objs[0][0][layer].id;
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			if (chunk->objs[i][k][layer].id != obj_id)
				return 0;
			if (chunk->objs[i][k][layer].data)
				return 0;
		}
	}
	return 1;
}

static int has_hitbox(chunk_t *chunk) {
	for (int i = 0; i < CHUNK_SIZE; i++) {
		for (int k = 0; k < CHUNK_SIZE; k++) {
			if (chunk->hitbox[i][k])
				return 1;
		}
	}
	return 0;
}

int obj_is_same(obj_t *a, obj_t *b) {
	if (a->id == 0 && b->id == 0)
		return 1;
	return a->id == b->id && a->data == b->data;
}

static int get_repetition(chunk_t *chunk, int layer, int x, int y) {
	int rep = 0;
	obj_t *obj = &chunk->objs[y][x][layer];
	while (y < CHUNK_SIZE && rep < 64) {
		if (obj_is_same(&chunk->objs[y][x][layer], obj))
			rep++;
		else
			break;
		x++;
		if (x >= CHUNK_SIZE) {
			x = 0;
			y++;
		}
	}
	return rep;
}

static void write_chunk_layer(chunk_t *chunk, int layer, FILE *f) {
	int x = 0;
	int y = 0;

	while (y < CHUNK_SIZE) {
		obj_t *obj = &chunk->objs[y][x][layer];
		uint8_t info = 0;
		info |= (obj->id == 0) << 7;
		info |= (obj->id && obj->data) << 6;
		int	rep = get_repetition(chunk, layer, x, y);
		printf("at %d %d rep %d\n", x, y, rep);
		info |= (rep - 1) & 0b00111111;
		fwrite(&info, 1, 1, f);
		if (obj->id != 0)
			fwrite(&obj->id, 1, 3, f);
		if (obj->data && obj->id)
			fwrite(&obj->data, 1, 4, f);

		x += rep;
		if (x >= CHUNK_SIZE) {
			y += x / CHUNK_SIZE;
			x %= CHUNK_SIZE;
		}
	}
}

static void write_hitbox(char hitbox[CHUNK_SIZE][CHUNK_SIZE], FILE *f) {
	int y = 0;
	int x = 0;
	while (y < CHUNK_SIZE) {
		uint8_t hit = 0;
		for (int i = 0; i < 8; i++) {
			hit <<= 1;
			hit |= hitbox[y][x];
			x++;
		}
		fwrite(&hit, 1, 1, f);
		if (x >= CHUNK_SIZE) {
			y++;
			x %= CHUNK_SIZE;
		}
	}
}

void game_world_save(world_t *world) {
	char *path = game_get_world_path(world->name);
	printf("path %s\n", path);
	FILE *f = fopen(path, "wb");
	printf("f %p %p %p\n", f, &world->width, &world->height);

	fwrite(&world->width, 1, sizeof(uint32_t), f);
	fwrite(&world->height, 1, sizeof(uint32_t), f);
	uint64_t *chunk_table = calloc( world->width * world->height, sizeof(uint64_t));
	long int chunk_table_offset = ftell(f);
	fwrite(chunk_table, world->width * world->height, sizeof(uint64_t), f);
	size_t current_chunk = 0;
	
	for (int cy = 0; cy < world->height; cy++) {
		for (int cx = 0; cx < world->width; cx++) {
			chunk_table[current_chunk++] = ftell(f);
			printf("crreunt %lu, %ld", current_chunk - 1, ftell(f));
			chunk_t *chunk = world->chunks[cy][cx];
			int layers_empty[3] = {0};
			int layers_is_repeat[3] = {0};
			layers_empty[0] = is_layer_empty(chunk, 0);
			layers_empty[1] = is_layer_empty(chunk, 1);
			layers_empty[2] = is_layer_empty(chunk, 2);
			if (layers_empty[0] && layers_empty[1] && layers_empty[2]) {
				fwrite("\x80", 1, 1, f);
				continue;
			}

			layers_is_repeat[0] = is_layer_repeat(chunk, 0);
			layers_is_repeat[1] = is_layer_repeat(chunk, 1);
			layers_is_repeat[2] = is_layer_repeat(chunk, 2);
			printf("empty %d %d %d\n", layers_empty[0], layers_empty[1], layers_empty[2]);
			printf("repeat %d %d %d\n", layers_is_repeat[0], layers_is_repeat[1], layers_is_repeat[2]);
			uint8_t chunk_info = 0;
			chunk_info |= (!layers_empty[0]) << 6;
			chunk_info |= (!layers_empty[1]) << 5;
			chunk_info |= (!layers_empty[2]) << 4;

			chunk_info |= (layers_is_repeat[0] && !layers_empty[0]) << 3;
			chunk_info |= (layers_is_repeat[1] && !layers_empty[1]) << 2;
			chunk_info |= (layers_is_repeat[2] && !layers_empty[2]) << 1;
			chunk_info |= has_hitbox(chunk);
			fwrite(&chunk_info, 1, 1, f);
			for (int i = 0; i < 3; i++) {
				if (layers_empty[i])
					continue;
				if (layers_is_repeat[i])
					fwrite(&chunk->objs[0][0][i].id, 1, 3, f);
				else
					write_chunk_layer(chunk, i, f);
			}
			if ((chunk_info & 0b1) == 1)
				write_hitbox(chunk->hitbox, f);
		}
	}
	long int old = ftell(f);
	fseek(f, chunk_table_offset, SEEK_SET);
	for (int i = 0; i < world->width * world->height; i++)
		printf("%lx\n", chunk_table[i]);
	fwrite(chunk_table, world->width * world->height, sizeof(uint64_t), f);
	fseek(f, old, SEEK_SET);
	fflush(f);
	fclose(f);
}
