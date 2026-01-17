#include "game.h"

typedef struct {
	SDL_Surface *img;
	int x;
	int y;
	int z;
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

static inline int add_entities(image_t *images, int end, entity_t *entities, int len) {
/*
(e - p) * TILE_SIZE + S / 2- T / 2

e * TILE_SIZE - p * TILE_SIZE +  S / 2 - T / 2
*/
	int off_x = -game_ctx->player->x * TILE_SIZE + GAME_WIDTH / 2 - TILE_SIZE / 2;
	int off_y = -game_ctx->player->y * TILE_SIZE + GAME_HEIGHT / 2 - TILE_SIZE / 2;
	for (int i = 0; i < len; i++) {
		entity_t *ent = &entities[i];
		SDL_Surface *texture = NULL;

		texture = game_get_sprite_texture(&ent->sprite);
		game_sprite_tick(&ent->sprite);
		if (!texture)
			continue;
		images[end].img = texture;
		images[end].x = ent->x * TILE_SIZE + off_x;
		images[end].y = ent->y * TILE_SIZE + off_y + TILE_SIZE;
		images[end].z = 0;
		end++;
	}
	return end;
}

static int compare(const void *a_ptr, const void *b_ptr) {
	const image_t *a = a_ptr;
	const image_t *b = b_ptr;
	if (!a->img && !b->img)
		return 0;
	if (!a->img)
		return -1;
	if (!b->img)
		return 1;
	if (a-> y == b->y)
		return a->x - b->x;
	return a->y - b->y;
}

static void render_images(image_t *images, int len) {
	qsort(images, len, sizeof(image_t), &compare);

	SDL_Rect rect = (SDL_Rect){0};
	for (int i = 0; i < len; i++) {
		if (!images[i].img)
			continue;
		rect.x = images[i].x + TILE_SIZE / 2 - images[i].img->w / 2;
		rect.y = images[i].y - images[i].img->h - images[i].z;
		rect.w = images[i].img->w;
		rect.h = images[i].img->h;
		SDL_BlitSurface(images[i].img, NULL, game_surface, &rect);
	}
}

void game_render_middle(chunk_t ***chunks, int size) {
	int offset_x = game_ctx->player->x;
	int offset_y = game_ctx->player->y;

	offset_x -= offset_x % CHUNK_SIZE;
	offset_y -= offset_y % CHUNK_SIZE;
	offset_x -= (size / 2) * CHUNK_SIZE + game_ctx->player->x;
	offset_y -= (size / 2) * CHUNK_SIZE + game_ctx->player->y;

	int frac_x = -(game_ctx->player->x - (int)game_ctx->player->x) * TILE_SIZE;  
	int frac_y = -(game_ctx->player->y - (int)game_ctx->player->y) * TILE_SIZE; 

	int len = count_len(chunks, size) + 1;
	image_t *images = malloc(sizeof(image_t) * len);
	memset(images, 0, sizeof(image_t) * len);
	int end = 0;
	for (int y = 0; y < size * CHUNK_SIZE; y++) {
		for (int x = 0; x < size * CHUNK_SIZE; x++) {
			chunk_t *chunk = chunks[y / CHUNK_SIZE][x / CHUNK_SIZE];
			if (!chunk)
				continue;
			int scr_x = GAME_WIDTH / 2 - TILE_SIZE / 2;
			int scr_y = GAME_HEIGHT / 2 - TILE_SIZE / 2;

			scr_x += (offset_x + x) * TILE_SIZE + frac_x;
			scr_y += (offset_y + y) * TILE_SIZE + frac_y;
			obj_t *obj = &chunk->objs[y % CHUNK_SIZE][x % CHUNK_SIZE][1];
			if (!obj->id)
				continue;

			SDL_Surface *texture = NULL;
	
			texture = game_get_sprite_texture(&obj->sprite);
			game_sprite_tick(&obj->sprite);
			if (!texture)
				continue;
			images[end].img = texture;
			images[end].x = scr_x + TILE_SIZE / 2;
			images[end].y = scr_y + TILE_SIZE + TILE_SIZE / 2;
			images[end].z = 0;
			end++;
		}
	}
	for (int i = 0; i < size; i++) {
		for (int k = 0; k < size; k++) {
			chunk_t *chunk = chunks[i][k];
			if (!chunk)
				continue;
			end = add_entities(images, end, chunk->entities, chunk->entities_len);
		}
	}
	if (!game_ctx->is_editor) {
		images[end].x = GAME_WIDTH / 2 - TILE_SIZE / 2; 
		images[end].y = GAME_HEIGHT / 2 + TILE_SIZE / 2; 
		images[end].z = 0;
		int action_duration = (game_ctx->actions[GAME_ACT_UP + game_ctx->player->dir] * 100 / GAME_FPS) % 100;
		if (game_ctx->actions[GAME_ACT_UP + game_ctx->player->dir] == 0)
			action_duration = 0;
		else {
    		action_duration = ((action_duration / 17) % 2) + 1;
		}
		images[end].img = game_ctx->player_textures[game_ctx->player->dir][action_duration];
	}
	render_images(images, len);
	free(images);
}	
