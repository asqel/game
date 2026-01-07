#include "game.h"

char *game_dir = "./";

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *game_surface = NULL;
SDL_Texture *texture = NULL;

int is_sdl_init = 0;
int is_ttf_init = 0;
int is_img_init = 0;

obj_info_t *obj_registry = NULL;
uint32_t obj_registry_len = 0;
texture_t *texture_registry = NULL;
uint32_t texture_registry_len = 0;
game_t *game_ctx = NULL;

sprite_registry_t *sprite_registry = NULL;
uint32_t sprite_registry_len = 0;


lua_State *lua_state = NULL;