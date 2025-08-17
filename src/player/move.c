#include "game.h"

typedef struct {
	double x;
	double y;
	double w;
	double h;
} hitbox_t;

#define H_HEIGHT 0.3
#define H_WIDTH 0.8

static hitbox_t get_player_hitbox() {
	hitbox_t res;
	res.x = game_ctx->player->x - H_WIDTH / 2;
	res.y = game_ctx->player->y + 0.5 - H_HEIGHT;
	res.w = H_WIDTH;
	res.h = H_HEIGHT;
	return res;
}

static void get_objs(char map[3][3], hitbox_t player_hit) {
	for (int i = 0; i < 3; i++)
		for (int k = 0; k < 3; k++)
			map[i][k] = 0;
	for (int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			int mx = k - 1 + (int)player_hit.x;
			int my = i - 1 + (int)player_hit.y;
			if (mx < 0 || my < 0)
				continue;
			int cy = my / CHUNK_SIZE;
			int cx = mx / CHUNK_SIZE;
			if (cx >= game_ctx->world->width || cy >= game_ctx->world->height || game_ctx->world->chunks[cy][cx] == NULL)
				continue;
			int obj_id = game_ctx->world->chunks[cy][cx]->objs[my % CHUNK_SIZE][mx % CHUNK_SIZE][1].id;
			if (obj_id > 0 && obj_id < obj_registry_len)
				map[i][k] = obj_registry[obj_id].has_hitbox;
			map[i][k] = map[i][k] || game_ctx->world->chunks[cy][cx]->hitbox[my % CHUNK_SIZE][mx % CHUNK_SIZE];
		}
	}
}

static int does_collide(hitbox_t a, hitbox_t b) {
	return !(a.x + a.w <= b.x ||
             b.x + b.w <= a.x ||
             a.y + a.h <= b.y ||
             b.y + b.h <= a.y);
}

int player_move(double x, double y) {
	if (game_ctx->is_editor) {
		if (x < 0 && (game_ctx->actions[GAME_ACT_LEFT] == 1 || game_ctx->actions[GAME_ACT_LEFT] > 15))
			game_ctx->player->x--;
		else if (x > 0 && (game_ctx->actions[GAME_ACT_RIGHT] == 1 || game_ctx->actions[GAME_ACT_RIGHT] > 15))
			game_ctx->player->x++;
		if (y < 0 && (game_ctx->actions[GAME_ACT_UP] == 1 || game_ctx->actions[GAME_ACT_UP] > 15))
			game_ctx->player->y--;
		else if (y > 0 && (game_ctx->actions[GAME_ACT_DOWN] == 1 || game_ctx->actions[GAME_ACT_DOWN] > 15))
			game_ctx->player->y++;
		return 1;
	}
	hitbox_t player_hit = get_player_hitbox();
	player_hit.x += x;
	player_hit.y += y;

	char map[3][3] = {{0}};
	get_objs(map, player_hit);
	for	(int i = 0; i < 3; i++) {
		for (int k = 0; k < 3; k++) {
			if (map[i][k] == 0)
				continue;
			hitbox_t obj_hit;
			obj_hit.x =  (k - 1) + (int)player_hit.x;
			obj_hit.y =  (i - 1) + (int)player_hit.y;
			obj_hit.w =  1;
			obj_hit.h =  1;
			if (does_collide(player_hit, obj_hit)) {
				if (x == 0 || y == 0)
					return 0;
				if (player_move(0, y) == 0)
					return player_move(x, 0);
				return 0;
			}
		}
	}
	game_ctx->player->x += x;
	game_ctx->player->y += y;
	return 1;
}
