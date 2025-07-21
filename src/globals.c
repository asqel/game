#include "game.h"

char *game_dir = "./";

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Surface *game_surface = NULL;
SDL_Texture *texture = NULL;

obj_info_t *obj_registry = NULL;
int obj_registry_len = 0;
texture_t *texture_registry = NULL;
int texture_registry_len = 0;
game_t *game_ctx = NULL;

char *game_error_str = NULL;
int game_error_is_special = GAME_ERROR_NONE; // like errno (doesnt use game_error_str)