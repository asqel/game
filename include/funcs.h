#ifndef GAME_FUNCS
#define GAME_FUNCS

#include "types.h"
#include <errno.h>

void	game_init(int argc, char **argv);
int		game_path_is_dir(char *path);
void	game_list_files(char *path, void (*callback)(const char *filepath));
int		game_path_is_file(char *path);

void	sdl_exit();
void	game_exit(int exit_code);

char	*game_get_error();
void	game_set_error(const char *error);
void	game_set_error_special(int error_code);
void	game_load_tx(const char *png_path, const char *tx_path);

void game_register_obj(char *name, int sprite_id);
int game_get_obj_id(char *name);

int game_texture_get_id(const char *name);
chunk_t *game_load_chunk(world_t *world, int x, int y);
world_t *game_load_world(char *name);

void update_screen();
void game_render_text(char *text, int x, int y, uint8_t r, uint8_t g, uint8_t b);
void game_render_strf(int x, int y, uint8_t r, uint8_t g, uint8_t b, char *text, ...);

void game_render();

void register_sprite(int *texture_ids, int texture_ids_len, int frame_len_tick, const char *name, uint8_t state);
sprite_t get_sprite(char *name);
sprite_t get_sprite_by_id(int sprite_id);

void game_pause_sprite(sprite_t *sprite);
void game_resume_sprite(sprite_t *sprite);
void game_set_sprite_loop(sprite_t *sprite, int loop);
void game_sprite_tick(sprite_t *sprite);
void game_sprite_reset(sprite_t *sprite);

SDL_Surface *game_get_sprite_texture(sprite_t *sprite);
int get_sprite_id(const char *name);

obj_t game_get_obj(int id);
void sprite_free();
void textures_free();
void objects_free();

void game_tick();
void update_event();
#endif
