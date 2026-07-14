#ifndef GAME_GLOBALS
#define GAME_GLOBALS

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "types.h"


//------- display
extern SDL_Window			*window;
extern SDL_Renderer			*renderer;
extern int					is_sdl_init;
extern int					is_img_init;

//------- registries
extern sprite_registry_t	*sprite_registry;
extern uint32_t				sprite_registry_len;

extern texture_t			*texture_registry;
extern uint32_t				texture_registry_len;
extern SDL_Texture			**atlases;
extern uint32_t				atlases_len;

extern obj_info_t			*obj_registry;
extern uint32_t				obj_registry_len;

//------- other
extern game_t				*game_ctx;
extern char					*game_dir;

extern char					game_lang[6];

//------- dialogue
extern dialogue_info_t		*dialogue_infos;
extern uint32_t				dialogue_infos_len;

//-------- lua
extern lua_State			*lua_state;

//-------- entities
extern entity_info_t		*entities_infos;
extern int					entities_infos_len;

#endif
