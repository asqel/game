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

static inline hitbox_t hitbox(double x, double y, double w, double h) {
	return (hitbox_t){x, y, w, h};
}

static void get_objs(hitbox_t map[5][5][2], hitbox_t player_hit) {
	for (int i = 0; i < 5; i++)
		for (int k = 0; k < 5; k++) {
			map[i][k][0] = (hitbox_t){0};
			map[i][k][1] = (hitbox_t){0};
		}
	for (int i = 0; i < 5; i++) {
		for (int k = 0; k < 5; k++) {
			int mx = k - 2 + (int)player_hit.x;
			int my = i - 2 + (int)player_hit.y;
			if (mx < 0 || my < 0)
				continue;
			int cy = my / CHUNK_SIZE;
			int cx = mx / CHUNK_SIZE;
			if (cx >= game_ctx->world->width || cy >= game_ctx->world->height || game_ctx->world->chunks[cy][cx] == NULL)
				continue;
			uint32_t obj_id = game_ctx->world->chunks[cy][cx]->objs[my % CHUNK_SIZE][mx % CHUNK_SIZE][1].id;
			if (obj_id > 0 && obj_id < obj_registry_len) {
				int hit_val = obj_registry[obj_id].has_hitbox;
				if (hit_val == 1)
					map[i][k][0] = hitbox(0, 0, 1, 1);
				else if (hit_val == 2) {
					double x = obj_registry[obj_id].hit_x;
					double y = obj_registry[obj_id].hit_y;
					double w = obj_registry[obj_id].hit_w;
					double h = obj_registry[obj_id].hit_h;
					map[i][k][0] = hitbox(x, y, w, h);
				}
			}
			if (game_ctx->world->chunks[cy][cx]->hitbox[my % CHUNK_SIZE][mx % CHUNK_SIZE])
				map[i][k][1] = hitbox(0, 0, 1, 1);
		}
	}
}

static int does_collide(hitbox_t a, hitbox_t b) {
	return (a.x + a.w >= b.x) && (a.x <= b.x + b.w) && (a.y + a.h  >= b.y) && (a.y <= b.y + b.h);
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

	hitbox_t map[5][5][2];
	get_objs(map, player_hit);
	for	(int i = 0; i < 5; i++) {
		for (int k = 0; k < 5; k++) {
			for (int l = 0; l < 2; l++) {
				hitbox_t obj_hit = map[i][k][l];
				if (obj_hit.w <= 0 && obj_hit.h <= 0)
					continue;
				obj_hit.x += (k - 2) + (int)player_hit.x;
				obj_hit.y +=  (i - 2) + (int)player_hit.y;
				if (does_collide(player_hit, obj_hit)) {
					if (x == 0 || y == 0)
						return 0;
					if (player_move(0, y) == 0)
						return player_move(x, 0);
					return 0;
				}
			}
		}
	}
	game_ctx->player->x += x;
	game_ctx->player->y += y;
	return 1;
}
