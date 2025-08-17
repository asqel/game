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
int obj_registry_len = 0;
texture_t *texture_registry = NULL;
int texture_registry_len = 0;
game_t *game_ctx = NULL;

char *game_error_str = NULL;
int game_error_is_special = GAME_ERROR_NONE; // like errno (doesnt use game_error_str)

sprite_registry_t *sprite_registry = NULL;
int sprite_registry_len = 0;