#include "game.h"

static inline void render_obj(obj_t *obj, int x, int y) {
	texture_t *texture = game_get_sprite_texture(&obj->sprite);
	game_sprite_tick(&obj->sprite);

	if (!texture)
		return ;

	SDL_Rect rect = (SDL_Rect){0};
	rect.x = x - texture->dest_w / 2;
	rect.y = y - texture->dest_h;
	rect.w = texture->dest_w;
	rect.h = texture->dest_h;
	SDL_RenderCopy(renderer, atlases[texture->atlas_idx], &texture->src_rect, &rect);
}

void game_render_background(chunk_t ***chunks, int size, double player_x, double player_y) {
	int player_middle_x = player_x * TILE_SIZE + TILE_SIZE / 2;
	int player_middle_y = player_y * TILE_SIZE + TILE_SIZE / 2;
	
	int offset_x = -player_middle_x + GAME_WIDTH / 2;
	int offset_y = -player_middle_y + GAME_HEIGHT / 2;

	int obj_top_left_x = (((int)player_x) / CHUNK_SIZE - size / 2) * CHUNK_SIZE;
	int obj_top_left_y = (((int)player_y) / CHUNK_SIZE - size / 2) * CHUNK_SIZE;

	int current_feet_y = obj_top_left_y * TILE_SIZE + offset_y + TILE_SIZE;
	for (int y = 0; y < size * CHUNK_SIZE; y++) {
		int current_feet_x = obj_top_left_x * TILE_SIZE + offset_x + TILE_SIZE / 2;

		for (int x = 0; x < size * CHUNK_SIZE; x++) {
			chunk_t *chunk = chunks[y / CHUNK_SIZE][x / CHUNK_SIZE];
			if (!chunk)
				goto next;

			obj_t *obj = &chunk->objs[y % CHUNK_SIZE][x % CHUNK_SIZE][0];
			if (!obj->id)
				goto next;

			render_obj(obj, current_feet_x, current_feet_y);
			next:
				current_feet_x += TILE_SIZE;
		}
		
		current_feet_y += TILE_SIZE;
	}
}
