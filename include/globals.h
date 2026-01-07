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
extern uint32_t				sprite_registry_len;

extern texture_t			*texture_registry;
extern uint32_t				texture_registry_len;

extern obj_info_t			*obj_registry;
extern uint32_t				obj_registry_len;

//------- other
extern game_t				*game_ctx;
extern char					*game_dir;

//------- editor
extern int					editor_obj_id;
extern int					editor_obj_layer;

extern char					game_lang[6];

//------- dialogue
extern dialogue_info_t		*dialogue_infos;
extern uint32_t				dialogue_infos_len;

//-------- lua
extern lua_State			*lua_state;

#endif
