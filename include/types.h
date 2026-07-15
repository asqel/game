#ifndef GAME_TYPES
#define GAME_TYPES

// !TODO rearange struct members to better pack

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

#define PRINT_ERR(...) fprintf(stderr, __VA_ARGS__)

#define GAME_MIN(X, Y) ((X) < (Y) ? (X) : (Y))
#define GAME_MAX(X, Y) ((X) > (Y) ? (X) : (Y))

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

#define DEFAULT_FRICTION 0.8
#define VELOCITY_EPSILON 0.005

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
typedef struct entity_t				entity_t;
typedef struct entity_info_t		entity_info_t;

struct gui_t {
	int data_ref;
	int update_ref;
	int render_ref;
	int free_ref;
};

struct texture_t {
	SDL_Rect src_rect;
	int dest_w;
	int dest_h;
	uint32_t atlas_idx;
	char name[TEXTURE_NAME_LENGTH + 1];
};

struct sprite_registry_t {
	uint32_t *textures_ids;
	uint16_t textures_ids_len;
	uint16_t frame_len_tick;
	char name[TEXTURE_NAME_LENGTH + 1];
	uint8_t state; // default state
};

struct sprite_t {
	uint32_t sprite_id; // id in registry
	uint16_t frame_idx;
	uint16_t current_frame_ttl; // tick to live
	uint8_t state;
};

struct obj_info_t {
	char name[OBJ_NAME_LENGTH + 1];
	uint32_t sprite_id;
	uint8_t has_hitbox; // 2 = custom
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
	// !TODO make this have 3 pointers so if no foreground NULL
	obj_t objs[CHUNK_SIZE][CHUNK_SIZE][3]; // 0: back, 1: middle, 2:top
	char hitbox[CHUNK_SIZE][CHUNK_SIZE];
	entity_t *entities;
	int entities_len;
	int rx; // real position (used by display)
	int ry;
};

struct world_t {
	chunk_t ***chunks;
	uint32_t width; // in chunks
	uint32_t height;
	char name[GAME_WORLD_NAME_MAX_LEN + 1];
};

struct player_t {
	int render_distance;
	gui_t *gui;
	int *entity_infos;
};

struct entity_t {
	uint32_t id; // id 0 == player

	double x;
	double y;
	double vx;
	double vy;
	double friction;
	int is_moving;
	int direction;

	double world_hitbox_x;
	double world_hitbox_y;
	double world_hitbox_w;
	double world_hitbox_h;

	double hp; // == 0 to die && id != 0
	sprite_t sprite;
	int data_ref;
	int lua_ref; // custom obj store cx, cy, idx in list
	int *lua_infos; // [3] (cx, cy, idx), if NULL skip this entity
};

struct entity_info_t {
	char name[OBJ_NAME_LENGTH + 1];
	double default_world_hitbox_x;
	double default_world_hitbox_y;
	double default_world_hitbox_w;
	double default_world_hitbox_h;
	double friction;
	int sprite_id;
	double hp;
	int on_tick_ref;
};

struct game_t {
	player_t *player;
	world_t *world;
	uint32_t actions[GAME_ACT_ENUM_MAX]; // how much the action is pressed
};

struct game_event_t {
	uint8_t type;
	uint8_t action; // 0 if not mapped
	uint32_t scancode; // SDL scancode
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
