#include "game.h"

typedef struct {
	texture_t *img;
	int x;
	int y;
	int z;
	int feet_y;
	int8_t depth;
} image_t;

static int count_len(chunk_t ***chunks, int size) {
	int res = 0;
	for (int i = 0; i < size; i++) {
		for (int k = 0; k < size; k++) {
			chunk_t *chunk = chunks[i][k];
			if (!chunk)
				continue;
			res += CHUNK_SIZE * CHUNK_SIZE + chunk->entities_len;
		}
	}
	return res;
}

static inline int add_entities(image_t *images, int end, entity_t *entities, int len, int offset_x, int offset_y, chunk_t *chunk) {
	for (int i = 0; i < len; i++) {
		entity_t *ent = &entities[i];

		texture_t *texture = game_get_sprite_texture(&ent->sprite);
		game_sprite_tick(&ent->sprite);

		if (!texture)
			continue;

		images[end].x = (ent->x - chunk->cx + chunk->sx) * TILE_SIZE + offset_x - texture->dest_w / 2;
		images[end].y = (ent->y - chunk->cy + chunk->sy) * TILE_SIZE + offset_y - texture->dest_h;
		images[end].z = 0;
		images[end].feet_y = ent->y + TILE_SIZE;
		images[end].img = texture;
		images[end].depth = ent->sprite_depth;

		end++;
	}
	return end;
}

static int compare(const void *a_ptr, const void *b_ptr) {
	const image_t *a = a_ptr;
	const image_t *b = b_ptr;
	if (a->depth < b->depth)
		return -1;
	if (a->depth > b->depth)
		return 1;
	if (!a->img && !b->img)
		return 0;
	if (!a->img)
		return -1;
	if (!b->img)
		return 1;
	if (a->feet_y < b->feet_y)
		return -1;
	if (a->feet_y > b->feet_y)
		return 1;
	if (a->x < b->x)
		return -1;
	if (a->x == b->x)
		return 0;
	return 1;
}

static void render_images(image_t *images, int len) {
	qsort(images, len, sizeof(image_t), &compare);

	SDL_Rect rect = (SDL_Rect){0};
	for (int i = 0; i < len; i++) {
		if (!images[i].img)
			continue;

		rect.x = images[i].x;
		rect.y = images[i].y - images[i].z;
		rect.w = images[i].img->dest_w;
		rect.h = images[i].img->dest_h;

		SDL_RenderCopy(renderer, atlases[images[i].img->atlas_idx], &images[i].img->src_rect, &rect);
	}
}

void game_render_middle(chunk_t ***chunks, int size, double player_x, double player_y) {
	int player_middle_x = player_x * TILE_SIZE + TILE_SIZE / 2;
	int player_middle_y = player_y * TILE_SIZE + TILE_SIZE / 2;
	
	int offset_x = -player_middle_x + GAME_WIDTH / 2;
	int offset_y = -player_middle_y + GAME_HEIGHT / 2;

	int obj_top_left_x = (((int)player_x) / CHUNK_SIZE - size / 2) * CHUNK_SIZE;
	int obj_top_left_y = (((int)player_y) / CHUNK_SIZE - size / 2) * CHUNK_SIZE;

	int len = count_len(chunks, size);
	image_t *images = malloc(sizeof(image_t) * len);
	memset(images, 0, sizeof(image_t) * len);
	int end = 0;

	int current_feet_y = obj_top_left_y * TILE_SIZE + offset_y + TILE_SIZE;
	for (int y = 0; y < size * CHUNK_SIZE; y++) {
		int current_feet_x = obj_top_left_x * TILE_SIZE + offset_x + TILE_SIZE / 2;

		for (int x = 0; x < size * CHUNK_SIZE; x++) {
			chunk_t *chunk = chunks[y / CHUNK_SIZE][x / CHUNK_SIZE];
			if (!chunk)
				goto next;

			obj_t *obj = &chunk->objs[y % CHUNK_SIZE][x % CHUNK_SIZE][1];
			if (!obj->id)
				goto next;

			texture_t *texture = game_get_sprite_texture(&obj->sprite);
			game_sprite_tick(&obj->sprite);

			if (!texture)
				goto next;

			images[end].img = texture;
			images[end].x = current_feet_x;
			images[end].y = current_feet_y;
			images[end].z = 0;
			images[end].feet_y = images[end].y;
			images[end].depth = 0;

			images[end].x -= texture->dest_w / 2;
			images[end].y -= texture->dest_h;

			end++;
			
			next:
				current_feet_x += TILE_SIZE;
		}

		current_feet_y += TILE_SIZE;
	}
	for (int i = 0; i < size; i++) {
		for (int k = 0; k < size; k++) {
			chunk_t *chunk = chunks[i][k];
			if (!chunk)
				continue;
			end = add_entities(images, end, chunk->entities, chunk->entities_len, offset_x, offset_y, chunk);
		}
	}
	render_images(images, len);
	free(images);
}	
