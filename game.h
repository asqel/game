#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <errno.h>

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)

#define GAME_WIDTH 960
#define GAME_HEIGHT 549
#define TEXTURE_NAME_LENGTH 256
#define OBJ_NAME_LENGTH 256
#define GAME_WORLD_NAME_MAX_LEN 100

#define CHUNK_SIZE 20

typedef struct chunk_t		chunk_t;
typedef struct world_t		world_t;
typedef struct player_t		player_t;
typedef struct obj_t		obj_t;
typedef struct game_t		game_t;
typedef struct obj_info_t	obj_info_t;
typedef struct texture_t	texture_t;

struct texture_t {
	SDL_Surface *surface;
	char name[TEXTURE_NAME_LENGTH + 1];
};

struct obj_info_t {
	char name[OBJ_NAME_LENGTH + 1];
	int frame_len;
	int *frame_ids;
	int frame_delay;
};

struct obj_t {
	uint32_t id;
	uint32_t data;
	int frame_idx;
	int next_frame_time;
};

struct chunk_t {
	obj_t objs[CHUNK_SIZE][CHUNK_SIZE][3]; // 0: back, 1: middle, 2:top
};

struct world_t {
	chunk_t ***chunks;
	int width; // in chunks
	int height;
	char name[GAME_WORLD_NAME_MAX_LEN + 1];
	char *path;
	void *infos;
};

struct player_t {
	double x;
	double y;
};

struct game_t {
	player_t *player;
	world_t *world;
};

extern char *game_dir;

extern SDL_Window	*window;
extern SDL_Renderer	*renderer;
extern SDL_Surface	*game_surface;
extern SDL_Texture	*texture;
extern obj_info_t	*obj_registry;
extern int			obj_registry_len;
extern texture_t	*texture_registry;
extern int			texture_registry_len;
extern char			*game_error_str;
extern int			game_error_is_special;
extern game_t		*game_ctx;

void	game_init(int argc, char **argv);
int		game_path_is_dir(char *path);
void	game_list_files(char *path, void (*callback)(const char *filepath));
int		game_path_is_file(char *path);

void	sdl_exit();
void	game_exit(int exit_code);

char	*game_get_error();
void	game_set_error(const char *error);
void	game_set_error_special(int error_code);
void	game_load_tx(const char *png_path, const char *tx_path);

void game_register_obj(char *name, int frame_len, int *frame_ids);
int game_get_obj_id(char *name);

int game_texture_get_id(const char *name);
chunk_t *game_load_chunk(world_t *world, int x, int y);

enum {
	GAME_ERROR_NONE = 0,

};

#endif
