#ifndef GAME_GLOBALS
#define GAME_GLOBALS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "types.h"


//------- display
extern SDL_Window			*window;
extern SDL_Renderer			*renderer;
extern SDL_Surface			*game_surface;
extern SDL_Texture			*texture;
extern int					is_sdl_init;
extern int					is_ttf_init;
extern int					is_img_init;

//------- registries
extern sprite_registry_t	*sprite_registry;
extern int					sprite_registry_len;

extern texture_t			*texture_registry;
extern int					texture_registry_len;

extern obj_info_t			*obj_registry;
extern int					obj_registry_len;

//------- errors
extern char					*game_error_str;
extern int					game_error_is_special;

//------- other
extern game_t				*game_ctx;
extern char					*game_dir;


#endif
