#ifndef GAME_GLOBALS
#define GAME_GLOBALS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "types.h"

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

#endif
