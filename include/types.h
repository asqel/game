#ifndef GAME_TYPES
#define GAME_TYPES

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)

#define TEXTURE_NAME_LENGTH 256
#define OBJ_NAME_LENGTH 256
#define GAME_WORLD_NAME_MAX_LEN 100
#define CHUNK_SIZE 20

#define GAME_WIDTH (1080)
#define GAME_HEIGHT (600)
#define TILE_SIZE (64)
#define GAME_FPS (60)

#define SPRITE_MASK_PAUSED (1)
#define SPRITE_MASK_LOOP (1 << 1)


enum {
	GAME_ACT_NONE,
	GAME_ACT_UP,
	GAME_ACT_RIGHT,
	GAME_ACT_DOWN,
	GAME_ACT_LEFT,
	GAME_ACT_INTERACT,
	GAME_ACT_CLOSE,
	GAME_ACT_EDIT_UP,
	GAME_ACT_EDIT_RIGHT,
	GAME_ACT_EDIT_DOWN,
	GAME_ACT_EDIT_LEFT,
	GAME_ACT_EDIT_PLACE,
	GAME_ACT_EDIT_REMOVE,
	GAME_ACT_EDIT_FILL,
	GAME_ACT_EDIT_EMPTY,
	GAME_ACT_EDIT_SAVE,
	GAME_ACT_ENUM_MAX
};

#define GAME_EVTY_INVALID 0
#define GAME_EVTY_PRESSED 1
#define GAME_EVTY_RELEASED 2
#define GAME_EVTY_TEXT 3

typedef struct chunk_t				chunk_t;
typedef struct world_t				world_t;
typedef struct player_t				player_t;
typedef struct obj_t				obj_t;
typedef struct game_t				game_t;
typedef struct obj_info_t			obj_info_t;
typedef struct texture_t			texture_t;
typedef struct sprite_registry_t	sprite_registry_t;
typedef struct sprite_t				sprite_t;
typedef struct gui_t				gui_t;
typedef struct game_event_t			game_event_t;
typedef struct dialogue_info_t		dialogue_info_t;
typedef struct dialogue_char_t		dialogue_char_t;
typedef struct c_lua_obj_t			c_lua_obj_t;
typedef struct entity_t				entity_t;
typedef struct entity_info_t		entity_info_t;

struct c_lua_obj_t {
	uint8_t is_lua;
	union {
		void *c;
		int lua_ref;
	};
};

struct gui_t {
	c_lua_obj_t data;
	c_lua_obj_t update;
	c_lua_obj_t render;
	c_lua_obj_t free;
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
	uint32_t sprite_id; // id in registry
	int frame_idx;
	int current_frame_ttl; // tick to live
	uint8_t state;
};

struct obj_info_t {
	char name[OBJ_NAME_LENGTH + 1];
	int sprite_id;
	int has_hitbox; // 2 = custom
	c_lua_obj_t interact;
	double hit_x;
	double hit_y;
	double hit_w;
	double hit_h;
};

struct obj_t {
	uint32_t id;
	uint32_t data;
	sprite_t sprite;
};

struct chunk_t {
	obj_t objs[CHUNK_SIZE][CHUNK_SIZE][3]; // 0: back, 1: middle, 2:top
	char hitbox[CHUNK_SIZE][CHUNK_SIZE];
	entity_t *entities;
	int entities_len;
};

struct world_t {
	chunk_t ***chunks;
	int width; // in chunks
	int height;
	char name[GAME_WORLD_NAME_MAX_LEN + 1];
};

struct player_t {
	double x;
	double y;
	uint8_t dir; // 0 up, 1 right, 2 down 3 left
	int render_distance;
	gui_t *gui;
};

struct entity_t {
	double x;
	double y;
	double vx;
	double vy;
	c_lua_obj_t data;
	uint32_t id;
	sprite_t sprite;
	double hitbox_x;
	double hitbox_y;
	double hitbox_w;
	double hitbox_h;
	double hp;
	int has_to_die;
};

struct entity_info_t {
	char name[OBJ_NAME_LENGTH + 1];
	int sprite_id;
	double default_hitbox_x;
	double default_hitbox_y;
	double default_hitbox_w;
	double default_hitbox_h;
	c_lua_obj_t on_tick; // return 1 to die
	double hp;
};

struct game_t {
	player_t *player;
	world_t *world;
	uint32_t actions[GAME_ACT_ENUM_MAX]; // how much the action is pressed
	SDL_Surface *player_textures[4][3];
	TTF_Font *fonts[1];
	size_t fonts_height[1];
	int is_editor;

	size_t env_len;
	char **env_vars;
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
	DIALOG_TEXT,
	DIALOG_VAR, // \$123. (dot to mark the end of the number)
	DIALOG_PAUSE, // \P2. (pause for 2 tick)
	DIALOG_EMOTE, // \E3.
	DIALOG_WAIT, // \W
	DIALOG_PX, // \X
	DIALOG_PY, // \Y
	DIALOG_CR, // \R
	DIALOG_CG, // \G
	DIALOG_CB, // \B
	DIALOG_STYLE, // \S
	DIALOG_LINE, // \n
	DIALOG_FONT, // \F
};

struct dialogue_char_t {
	uint8_t type;
	union {
		size_t text[2]; // [0] idx, [1] len
		size_t var;
		size_t pause;
		size_t emote;
		int px;
		int py;
		uint32_t r;
		uint32_t g;
		uint32_t b;
		size_t style;
		size_t font;
	};
};

struct dialogue_info_t {
	char *original;
	dialogue_char_t *text;
	size_t len;
	char id[256];
};

#endif
