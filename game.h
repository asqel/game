#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

typedef struct chunk_t chunk_t;
typedef struct world_t world_t;
typedef struct player_t player_t;

struct chunk_t {
	uint32_t objs[3][20][20]; // 0: back, 1: middle, 2:top
	void     *obj_infos[3][20][20];
};

struct world_t {
	chunk_t ***chunks;
	char *name;
	char *dir_path;
	void *infos;
};

struct player_t {
	double x;
	double y;
};

struct game_t {
	player_t player;
	world_t world;
};

extern char *game_dir;

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Surface *gameSurface;
extern SDL_Texture *texture;

void game_init(int argc, char **argv);
int game_path_is_dir(char *path);

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__);

#define GAME_WIDTH 960
#define GAME_HEIGHT 549

#endif
