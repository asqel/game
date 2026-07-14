#ifndef GAME_FUNCS
#define GAME_FUNCS

#include "types.h"
#include <errno.h>
#include <stdnoreturn.h>

//------- init / exit
void		game_init(int argc, char **argv);
int			init_sdl();
void		init_textures();
void		init_sprite();
void init_objects();
void		init_ctx(int argc, char **argv);
void		sdl_exit();
noreturn void		game_exit(int exit_code);
void		sprite_free();
void		textures_free();
void		objects_free();
int			dialogue_init();
void		game_free_dialogues();
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
void add_texture(const char *name, SDL_Rect *src_rect, int dest_w, int dest_h, uint32_t atlas, const char *path);
uint32_t add_atlas(SDL_Texture *tx);

//------- sprite
sprite_t	get_sprite(char *name);
sprite_t	get_sprite_by_id(uint32_t sprite_id);
void		game_pause_sprite(sprite_t *sprite);
void		game_resume_sprite(sprite_t *sprite);
void		game_set_sprite_loop(sprite_t *sprite, int loop);
void		game_sprite_tick(sprite_t *sprite);
void		game_sprite_reset(sprite_t *sprite);
uint32_t	get_sprite_id(const char *name);
texture_t *game_get_sprite_texture(sprite_t *sprite);

//------- obj
uint32_t	game_get_obj_id(char *name);
obj_t		game_get_obj(uint32_t id);

//------- register
void		register_sprite(uint32_t *texture_ids, int texture_ids_len, int frame_len_tick, const char *name, uint8_t state);
void 		register_obj(char *name, uint32_t sprite_id, int has_hit, double hitx, double hity, double hitw, double hith);


//------- render
void		game_render();
void		update_screen();
void		game_render_background(chunk_t *** chunks, int size, double player_x, double player_y);
void		game_render_middle(chunk_t *** chunks, int size, double player_x, double player_y);
void		game_render_foreground(chunk_t *** chunks, int size, double player_x, double player_y);

//------- event
void		update_event();
int			game_action_pressed_hold(int action, int duration);
void		game_tick();

//------- chunk
chunk_t		*world_new_chunk(int x, int y, world_t *world); // chunk co
chunk_t		*world_new_chunk_at(int x, int y, world_t *world); // obj co
chunk_t 	*get_chunk(int x, int y, world_t *world);
chunk_t 	*get_chunk_at(int x, int y, world_t *world);

//------- world
world_t		*game_load_world(char *name);
obj_t		world_get_obj(world_t *world, int x, int y, int layer);
int			world_get_hitbox(world_t *world, int x, int y);
void		world_set_hitbox(world_t *world, int x, int y, int state);
void		world_set_obj(world_t *world, int x, int y, int layer, obj_t obj); 
int			game_world_exists(char *name);
void		game_world_save(world_t *world);

//------- gui

void		game_tick_gui();
void		game_close_gui();
void		game_render_gui();
gui_t		*game_open_gui(int data_ref, int update_ref, int render_ref, int free_ref);

//-------- utils
char *read_file(char *path);

//-------- dialogue
dialogue_info_t parse_dialogue(char *str);
int dialogue_register(char *id, char *val);
int parse_lang(char *text);
size_t get_dialogue_id(const char *name);
dialogue_info_t *get_dialogue_by_id(size_t id);

//-------- entities
entity_info_t *get_entity_info(uint32_t id);
void entities_tick();
uint32_t entity_register(char *name, int sprite_id, int tick_ref, double hp, double hit[4], double friction);
entity_t *entity_add(uint32_t id, int x, int y, world_t *world);
void entity_move_collide(entity_t *ent, double vx, double vy, int *to_remove);

//-------- lua funcs
int lua_func_register_obj(lua_State *l); 
int lua_func_register_obj2(lua_State *l);
int lua_func_register_sprite(lua_State *l);
int lua_func_get_sprite_id(lua_State *l);
int lua_func_launch_file(lua_State *l);
int lua_func_time(lua_State *l);
int lua_func_get_action(lua_State *l);
int lua_func_set_action(lua_State *l);
int lua_func_open_gui(lua_State *l);
int lua_func_close_gui(lua_State *l);
int lua_func_register_entity(lua_State *l);
int lua_func_entity_add(lua_State *l);
int lua_func_read_save(lua_State *l);
int lua_func_set_entity_func(lua_State *l);
int lua_func_set_entity_velocity(lua_State *l);
int lua_func_get_entity_ismoving(lua_State *l);

//--------- time
void game_loop_start();
void game_loop_end();
int game_get_fps();
void game_set_fps_target(uint64_t target);
uint64_t game_get_fps_target();
int64_t game_get_time();

#endif
