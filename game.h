#ifndef GAME_H
#define GAME_H

#include <stdint.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <errno.h>
#include <stdarg.h>

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)

#define GAME_WIDTH 960
#define GAME_HEIGHT 540
#define TEXTURE_NAME_LENGTH 256
#define OBJ_NAME_LENGTH 256
#define GAME_WORLD_NAME_MAX_LEN 100
#define TILE_SIZE 64
#define GAME_FPS 30
#define CHUNK_SIZE 20

#define SPRITE_MASK_PAUSED (1)
#define SPRITE_MASK_LOOP (1 << 1)

enum {
	GAME_ACT_UP,
	GAME_ACT_RIGHT,
	GAME_ACT_DOWN,
	GAME_ACT_LEFT,
	GAME_ACT_ENUM_MAX
};

typedef struct chunk_t		chunk_t;
typedef struct world_t		world_t;
typedef struct player_t		player_t;
typedef struct obj_t		obj_t;
typedef struct game_t		game_t;
typedef struct obj_info_t	obj_info_t;
typedef struct texture_t	texture_t;
typedef struct sprite_registry_t sprite_registry_t;
typedef struct sprite_t	sprite_t;
typedef struct gui_t		gui_t;

struct gui_t {
	void *data;
	void (*init)(gui_t *self);
	void (*render)(gui_t *self);
	void (*handle_event)(gui_t *self, SDL_Event *event);
	void (*free)(gui_t *self);
	void (*update)(gui_t *self);
};

struct texture_t {
	SDL_Surface *surface;
	char name[TEXTURE_NAME_LENGTH + 1];
};

struct sprite_registry_t {
	int *textures_ids;
	int textures_ids_len;
	int frame_len_tick;
	char name[TEXTURE_NAME_LENGTH + 1];
	uint8_t state; // default state
};

struct sprite_t {
	int sprite_id; // id in registry
	int frame_idx;
	int current_frame_ttl; // tick to live
	uint8_t state;
};

struct obj_info_t {
	char name[OBJ_NAME_LENGTH + 1];
	int sprite_id;
};

struct obj_t {
	uint32_t id;
	uint32_t data;
	sprite_t sprite;
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
	uint8_t dir; // 0 up, 1 right, 2 down 3 left
	gui_t *guis;
	int guis_len;
};

struct game_t {
	player_t *player;
	world_t *world;
	uint32_t actions[GAME_ACT_ENUM_MAX]; // how much the action is pressed
	SDL_Surface *player_textures[4][3];
	TTF_Font *fonts[1];
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

extern sprite_registry_t *sprite_registry;
extern int sprite_registry_len;

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

void game_register_obj(char *name, int sprite_id);
int game_get_obj_id(char *name);

int game_texture_get_id(const char *name);
chunk_t *game_load_chunk(world_t *world, int x, int y);
world_t *game_load_world(char *name);


void update_screen();
void game_render_text(char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void game_render_strf(int x, int y, uint8_t r, uint8_t g, uint8_t b, char *text, ...);

void game_render();


void register_sprite(int *texture_ids, int texture_ids_len, int frame_len_tick, const char *name, uint8_t state);
sprite_t get_sprite(char *name);
sprite_t get_sprite_by_id(int sprite_id);

void game_pause_sprite(sprite_t *sprite);
void game_resume_sprite(sprite_t *sprite);
void game_set_sprite_loop(sprite_t *sprite, int loop);
void game_sprite_tick(sprite_t *sprite);
void game_sprite_reset(sprite_t *sprite);

SDL_Surface *game_get_sprite_texture(sprite_t *sprite);
int get_sprite_id(const char *name);

obj_t game_get_obj(int id);
enum {
	GAME_ERROR_NONE = 0,

};

#endif
