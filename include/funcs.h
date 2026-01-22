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
int			dialogue_init();
void		dialogue_exit();
int			init_lua();

//------- file / path
int			game_path_is_dir(char *path);
void		game_list_files(char *path, void (*callback)(const char *filepath));
int			game_path_is_file(char *path);
char *game_get_world_path(char *name);
void normalize_path(char *str);

//------- error
char		*game_get_error();
void		game_set_error(const char *error);
void		game_set_error_special(int error_code);

//------- texture
void		game_load_tx(const char *png_path, const char *tx_path);
uint32_t	game_texture_get_id(const char *name);

//------- sprite
sprite_t	get_sprite(char *name);
sprite_t	get_sprite_by_id(uint32_t sprite_id);
void		game_pause_sprite(sprite_t *sprite);
void		game_resume_sprite(sprite_t *sprite);
void		game_set_sprite_loop(sprite_t *sprite, int loop);
void		game_sprite_tick(sprite_t *sprite);
void		game_sprite_reset(sprite_t *sprite);
uint32_t	get_sprite_id(const char *name);
SDL_Surface	*game_get_sprite_texture(sprite_t *sprite);

//------- obj
uint32_t	game_get_obj_id(char *name);
obj_t		game_get_obj(uint32_t id);

//------- register
void		game_register_obj(char *name, uint32_t sprite_id, int hithox, void *interact);
void game_register_obj_w_hit(char *name, uint32_t sprite_id, void *interact, double hitx, double hity, double hitw, double hith);
void		register_sprite(uint32_t *texture_ids, int texture_ids_len, int frame_len_tick, const char *name, uint8_t state);
void game_register_obj_full(char *name, uint32_t sprite_id, c_lua_obj_t func, int has_hit, double hitx, double hity, double hitw, double hith);


//------- render
void		game_render_text(char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void		game_render_strf(int x, int y, uint8_t r, uint8_t g, uint8_t b, char *text, ...);
void		game_render(uint32_t fps);
void		update_screen();
void		game_render_background(chunk_t *** chunks, int size);
void		game_render_middle(chunk_t *** chunks, int size);
void		game_render_foreground(chunk_t *** chunks, int size);

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
void		game_world_save(world_t *world);

//------- gui

void		game_tick_gui();
void		game_close_gui();
void		game_render_gui();
gui_t *game_open_gui(void *data, void (*update)(gui_t *self), void (*render)(gui_t *self), void (*free)(gui_t *self));
gui_t *game_open_gui2(c_lua_obj_t data, c_lua_obj_t update, c_lua_obj_t render, c_lua_obj_t free);
//-------- editor
void		game_editor_tick();

//-------- utils
char *read_file(char *path);

//-------- dialogue
dialogue_info_t parse_dialogue(char *str);
int dialogue_register(char *id, char *val);
int parse_lang(char *text);
void open_dialogue(char *name, int (*on_end)(gui_t *self), size_t id_len, ...);
void open_dialogue2(char *name, c_lua_obj_t on_end, size_t id_len, size_t *ids);
void display_dialogue(dialogue_info_t *dialogue, size_t len, size_t sublen, int width, int height, size_t style, int x, int y, size_t font);
size_t get_dialogue_id(const char *name);
dialogue_info_t *get_dialogue_by_id(size_t id);

//-------- entities
entity_info_t *get_entity_info(uint32_t id);
void entities_tick();

//-------- lua funcs
int lua_func_register_obj(lua_State *l); 
int lua_func_register_obj2(lua_State *l);
int lua_func_register_sprite(lua_State *l);
int lua_func_get_sprite_id(lua_State *l);
int lua_func_launch_file(lua_State *l);
int lua_func_open_dialogue(lua_State *L);
int lua_func_open_gui(lua_State *l);
int lua_func_draw_text(lua_State *l);
int lua_func_time(lua_State *l);
int lua_func_get_action(lua_State *l);
int lua_func_set_action(lua_State *l);
int lua_func_close_gui(lua_State *l);

//--------- time
void game_loop_start();
void game_loop_end();
int game_get_fps();
void game_set_fps_target(uint64_t target);
uint64_t game_get_fps_target();
int64_t game_get_time();

#endif
