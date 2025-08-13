#include "game.h"

typedef struct {
	double x;
	double y;
	double w;
	double h;
} hitbox_t;

static hitbox_t get_player_hitbox() {
	hitbox_t res;
	res.x = game_ctx->player->x + TILE_SIZE / 2;
	res.y = game_ctx->player->y + TILE_SIZE / 2;
	res.w = TILE_SIZE;
	res.h = TILE_SIZE;
	return res;
}

int player_move(double x, double y) {
	hitbox_t player_hit = get_player_hitbox();
	char map[3][3] = {{0}};
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			int mx = k - 1 + (int)game_ctx->player->x;
			int my = i - 1 + (int)game_ctx->player->y;
			if (mx < 0 || my < 0)
				continue;
			int cy = my / CHUNK_SIZE;
			int cx = mx / CHUNK_SIZE;
			if (cx >= game_ctx->world->width || cy >= game_ctx->world->height || game_ctx->world->chunks[cy][cx] == NULL)
				continue;
			int obj = game_ctx->world->chunks[cy][cx]->objs[my - cy][mx - cx][1].id;
			map[k][i] = game_ctx->world->chunks[cy][cx]->hitbox[my - cy][mx - cx];
			if (obj <= 0 || obj >= obj_registry_len)
				continue;
			map[k][i] = map[k][i] || obj_registry[obj].has_hitbox; 
		}
	}
	int dirx = 1;
	int diry = 1;
	if (x < 0)
		dirx = -1;
	if (y < 0)
		diry = -1;
	if (map[diry + 1][dirx + 1])
		return 0;
	game_ctx->player->x += x;
	game_ctx->player->y += y;
	return 1;

}
