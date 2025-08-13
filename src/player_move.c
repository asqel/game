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

int player_move_left() {
	hitbox_t player_hit = get_player_hitbox();
	int world_hit[3] = {0};

}
