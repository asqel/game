#ifndef GAME_TYPES
#define GAME_TYPES

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)

#define TEXTURE_NAME_LENGTH 256
#define OBJ_NAME_LENGTH 256
#define GAME_WORLD_NAME_MAX_LEN 100
#define CHUNK_SIZE 20

#define GAME_WIDTH 960
#define GAME_HEIGHT 540
#define TILE_SIZE 64
#define GAME_FPS 30

#define SPRITE_MASK_PAUSED (1)
#define SPRITE_MASK_LOOP (1 << 1)


enum {
	GAME_ACT_NONE,
	GAME_ACT_UP,
	GAME_ACT_RIGHT,
	GAME_ACT_DOWN,
	GAME_ACT_LEFT,
	GAME_ACT_CLOSE,
	GAME_ACT_ENUM_MAX
};

#define GAME_EVTY_INVALID 0
#define GAME_EVTY_PRESSED 1
#define GAME_EVTY_RELEASED 2
#define GAME_EVTY_TEXT 3

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
typedef struct game_event_t game_event_t;

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
	int has_hitbox;
};

struct obj_t {
	uint32_t id;
	uint32_t data;
	sprite_t sprite;
};

struct chunk_t {
	obj_t objs[CHUNK_SIZE][CHUNK_SIZE][3]; // 0: back, 1: middle, 2:top
	char hitbox[CHUNK_SIZE][CHUNK_SIZE];
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

struct game_event_t {
	uint8_t type;
	union {
		struct {
			uint8_t action; // 0 if not mapped
			uint32_t scancode; // SDL scancode
		} key;
		char text[32];
	};
};

enum {
	GAME_ERROR_NONE = 0,

};

#endif
