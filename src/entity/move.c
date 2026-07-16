#include "game.h"

typedef struct {
	double x;
	double y;
	double w;
	double h;
} hitbox_t;

typedef struct {
	int hit;
	double t;
	double nx;
	double ny;
} info_t;

static inline void normalize_coord(double *co, int lim) {
	while (*co < 0)
		*co += lim;
	while (*co >= lim)
		*co -= lim;
}

static inline void normalize_coord_int(int *co, int lim) {
	while (*co < 0)
		*co += lim;
	while (*co >= lim)
		*co -= lim;
}

static inline void swept_aabb(hitbox_t *moving_hit, double dx, double dy, hitbox_t *obj_hit, info_t *info) {
	double x_inv_entry = 0;
	double x_inv_exit = 0;
	double y_inv_entry = 0;
	double y_inv_exit = 0;

	if (dx > 0) {
		x_inv_entry = obj_hit->x - (moving_hit->x + moving_hit->w);
		x_inv_exit = (obj_hit->x + obj_hit->w) - moving_hit->x;
	}
	else {
		x_inv_exit = obj_hit->x - (moving_hit->x + moving_hit->w);
		x_inv_entry = (obj_hit->x + obj_hit->w) - moving_hit->x;
	}

	if (dy > 0) {
		y_inv_entry = obj_hit->y - (moving_hit->y + moving_hit->h);
		y_inv_exit = (obj_hit->y + obj_hit->h) - moving_hit->y;
	}
	else {
		y_inv_exit = obj_hit->y - (moving_hit->y + moving_hit->h);
		y_inv_entry = (obj_hit->y + obj_hit->h) - moving_hit->y;
	}

	double x_entry = (dx == 0) ? -INFINITY : x_inv_entry / dx;
    double x_exit = (dx == 0) ?  INFINITY : x_inv_exit / dx;

    double y_entry = (dy == 0) ? -INFINITY : y_inv_entry / dy;
    double y_exit = (dy == 0) ?  INFINITY : y_inv_exit / dy;

	double entry = x_entry;
	if (y_entry > entry)
		entry = y_entry;
	
	double exit = x_exit;
	if (y_exit < exit)
		exit = y_exit;
	
	if (entry > exit || entry < 0 ||  entry > 1)
		return ;
	
	info->hit = 1;
	info->t = entry;
	
	//if (x_entry > y_entry)
	//	info->nx = (dx > 0) ? -1 : 1;
	//else
	//	info->ny = (dy > 0) ? -1 : 1;
	if (x_entry > y_entry) {
    info->nx = (dx > 0) ? -1 : 1;
    info->ny = 0;
}
else {
    info->nx = 0;
    info->ny = (dy > 0) ? -1 : 1;
}
}

static inline int get_obj_hit(hitbox_t *hit, int x, int y, int width_as_block, int height_as_block) {
	normalize_coord_int(&x, width_as_block);
	normalize_coord_int(&y, height_as_block);

	int cx = x / CHUNK_SIZE;
	int cy = y / CHUNK_SIZE;
	int ix = x % CHUNK_SIZE;
	int iy = y % CHUNK_SIZE;

	if (!game_ctx->world->chunks[cy][cx])
		return 1;
	obj_t *obj = &game_ctx->world->chunks[cy][cx]->objs[iy][ix][1];
	obj_info_t *info = &obj_registry[obj->id];
	if (info->has_hitbox == 0)
		return 1;
	
	hit->x = x;
	hit->y = y;
	if (info->has_hitbox == 1) {
		hit->w = 1;
		hit->h = 1;
		return 0;
	}
	hit->w = info->hit_w;
	hit->h = info->hit_h;
	hit->x += info->hit_x;
	hit->y += info->hit_y;
	return 0;
}

static inline void move(hitbox_t *ent_hit, double dx, double dy, int width_as_block, int height_as_block, info_t *best) {
	best->t = 1;
	best->hit = 0;
	best->nx = 0;
	best->ny = 0;

	double min_x = GAME_MIN(ent_hit->x, ent_hit->x + dx);
	double min_y = GAME_MIN(ent_hit->y, ent_hit->y + dy);
	double max_x = GAME_MAX(ent_hit->x + ent_hit->w, ent_hit->x + dx + ent_hit->w);
	double max_y = GAME_MAX(ent_hit->y + ent_hit->h, ent_hit->y + dy + ent_hit->h);

	for (int y = min_y; y <= max_y; y++) {
		for (int x = min_x; x <= max_x; x++) {
			hitbox_t obj_hit = {0};
			if (get_obj_hit(&obj_hit, x, y, width_as_block, height_as_block))
				continue;

			info_t ret = {0};
			ret.t = 1;
			swept_aabb(ent_hit, dx, dy, &obj_hit, &ret);

			if (ret.hit && ret.t <= best->t)
				*best = ret;
		}
	}
}

void entity_move_collide(entity_t *ent, double vx, double vy, int *to_remove) {
	if (vx == 0 && vy == 0)
		return ;
	int width_as_block = game_ctx->world->width * CHUNK_SIZE;
	int height_as_block = game_ctx->world->height * CHUNK_SIZE;

	normalize_coord(&ent->x, width_as_block);
	normalize_coord(&ent->y, height_as_block);

	hitbox_t ent_hit = {0};
	ent_hit.x = ent->x - ent->world_hitbox_w / 2 + ent->world_hitbox_x;
	ent_hit.y = ent->y - ent->world_hitbox_h + ent->world_hitbox_y;
	ent_hit.w = ent->world_hitbox_w;
	ent_hit.h = ent->world_hitbox_h;

	info_t info = {0};
	info.t = 1;
	move(&ent_hit, vx, vy, width_as_block, height_as_block, &info);
	//ent->x += vx * info.t;
	//ent->y += vy * info.t;
	//if (info.hit) {
	//	if (info.nx)
	//		vx = 0;
	//	if (info.ny)
	//		vy = 0;
	//	entity_move_collide(ent, vx, vy, to_remove);
	//}
	info.t -= 0.001;
	if (info.t < 0)
		info.t = 0;
	double remain = 1.0 - info.t;

	ent->x += vx * info.t;
	ent->y += vy * info.t;
	
	if (info.hit) {
	    vx *= remain;
	    vy *= remain;
	
	    if (info.nx)
	        vx = 0;
	
	    if (info.ny)
	        vy = 0;
	
	    entity_move_collide(ent, vx, vy, to_remove);
	}
}
