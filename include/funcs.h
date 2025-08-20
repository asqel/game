#ifndef GAME_FUNCS
#define GAME_FUNCS

#include "types.h"
#include <errno.h>

//------- init / exit
void		game_init(int argc, char **argv);
int			init_sdl();
int			init_textures();
int			init_sprites();
int			init_objects();
void		init_ctx(int argc, char **argv);
void		sdl_exit();
void		game_exit(int exit_code);
void		sprite_free();
void		textures_free();
void		objects_free();

//------- file / path
int			game_path_is_dir(char *path);
void		game_list_files(char *path, void (*callback)(const char *filepath));
int			game_path_is_file(char *path);
//------- error
char		*game_get_error();
void		game_set_error(const char *error);
void		game_set_error_special(int error_code);

//------- texture
void		game_load_tx(const char *png_path, const char *tx_path);
int			game_texture_get_id(const char *name);

//------- sprite
sprite_t	get_sprite(char *name);
sprite_t	get_sprite_by_id(int sprite_id);
void		game_pause_sprite(sprite_t *sprite);
void		game_resume_sprite(sprite_t *sprite);
void		game_set_sprite_loop(sprite_t *sprite, int loop);
void		game_sprite_tick(sprite_t *sprite);
void		game_sprite_reset(sprite_t *sprite);
int			get_sprite_id(const char *name);
SDL_Surface	*game_get_sprite_texture(sprite_t *sprite);

//------- obj
int			game_get_obj_id(char *name);
obj_t		game_get_obj(int id);

//------- register
void		game_register_obj(char *name, int sprite_id, int hithox, void *interact);
void		register_sprite(int *texture_ids, int texture_ids_len, int frame_len_tick, const char *name, uint8_t state);


//------- render
void		game_render_text(char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void		game_render_strf(int x, int y, uint8_t r, uint8_t g, uint8_t b, char *text, ...);
void		game_render();
void		update_screen();

//------- event
void		update_event();
int			game_action_pressed_hold(int action, int duration);
void		game_enable_text_input();
void		game_disable_text_input();
void		game_tick();

//------- player
void		player_tick();
int			player_move(double x, double y);
void		player_interact();

//------- chunk
chunk_t		*game_load_chunk(world_t *world, int x, int y);
chunk_t		*world_get_chunk(world_t *world, int cx, int cy);

//------- world
world_t		*game_load_world(char *name);
obj_t		*world_get_obj_at(world_t *world, int x, int y, int layer);
int			game_world_exists(char *name);

//------- gui

void		game_tick_gui();
void		game_close_gui();
void		game_render_gui();
void		game_send_event_gui(game_event_t *ev);
int			game_is_gui_open();

//-------- editor
void		game_editor_tick();

#endif
