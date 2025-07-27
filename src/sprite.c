#include "game.h"

void register_sprite(int *texture_ids, int texture_ids_len, int frame_len_tick, const char *name, uint8_t state) {
	if (strlen(name) > TEXTURE_NAME_LENGTH) {
		game_set_error("Sprite name exceeds maximum length");
		return;
	}
	sprite_registry_t *new_sprite = realloc(sprite_registry, sizeof(sprite_registry_t) * (sprite_registry_len + 1));
	if (!new_sprite) {
		game_set_error("Failed to allocate memory for sprite registry");
		return;
	}
	sprite_registry = new_sprite;

	sprite_registry[sprite_registry_len].textures_ids = malloc(texture_ids_len * sizeof(int));
	if (!sprite_registry[sprite_registry_len].textures_ids) {
		game_set_error("Failed to allocate memory for sprite textures");
		return;
	}

	memcpy(sprite_registry[sprite_registry_len].textures_ids, texture_ids, texture_ids_len * sizeof(int));
	sprite_registry[sprite_registry_len].textures_ids_len = texture_ids_len;
	sprite_registry[sprite_registry_len].frame_len_tick = frame_len_tick;
	sprite_registry[sprite_registry_len].state = state;
	strcpy(sprite_registry[sprite_registry_len].name, name);
	sprite_registry_len++;
}

sprite_t get_sprite(char *name) {
	for (int i = 0; i < sprite_registry_len; i++) {
		if (strcmp(sprite_registry[i].name, name) == 0) {
			sprite_t sprite;
			sprite.sprite_id = i;
			sprite.frame_idx = 0;
			sprite.current_frame_ttl = sprite_registry[i].frame_len_tick;
			sprite.state = sprite_registry[i].state;
			return sprite;
		}
	}
	sprite_t empty_sprite = {-1, -1, -1, 0 };
	return empty_sprite;
}

sprite_t get_sprite_by_id(int sprite_id) {
	if (sprite_id < 0 || sprite_id >= sprite_registry_len) {
		sprite_t empty_sprite = {-1, -1, -1, 0 };
		return empty_sprite;
	}
	sprite_t sprite;
	sprite.sprite_id = sprite_id;
	sprite.frame_idx = 0;
	sprite.current_frame_ttl = sprite_registry[sprite_id].frame_len_tick;
	sprite.state = sprite_registry[sprite_id].state;
	return sprite;
}

void game_pause_sprite(sprite_t *sprite) {
	if (sprite)
		sprite->state |= SPRITE_MASK_PAUSED;
}

void game_resume_sprite(sprite_t *sprite) {
	if (sprite)
		sprite->state &= ~SPRITE_MASK_PAUSED;
}

void game_set_sprite_loop(sprite_t *sprite, int loop) {
	if (sprite) {
		if (loop)
			sprite->state |= SPRITE_MASK_LOOP;
		else
			sprite->state &= ~SPRITE_MASK_LOOP;
	}
}

void game_sprite_tick(sprite_t *sprite) {
	if (!sprite || sprite->sprite_id < 0 || sprite->sprite_id >= sprite_registry_len)
		return;

	if (sprite->state & SPRITE_MASK_PAUSED)
		return;
	if (sprite->current_frame_ttl > 0)
		sprite->current_frame_ttl--;
	if (sprite->current_frame_ttl <= 0) {
		sprite->frame_idx++;
		if (sprite->frame_idx >= sprite_registry[sprite->sprite_id].textures_ids_len) {
			if (sprite->state & SPRITE_MASK_LOOP)
				sprite->frame_idx = 0;
			else
				sprite->frame_idx = sprite_registry[sprite->sprite_id].textures_ids_len - 1;
		}
		sprite->current_frame_ttl = sprite_registry[sprite->sprite_id].frame_len_tick;
	}
}

void game_sprite_reset(sprite_t *sprite) {
	if (sprite && sprite->sprite_id >= 0 && sprite->sprite_id < sprite_registry_len) {
		sprite->frame_idx = 0;
		sprite->current_frame_ttl = sprite_registry[sprite->sprite_id].frame_len_tick;
		sprite->state &= ~SPRITE_MASK_PAUSED;
	}
}

SDL_Surface *game_get_sprite_texture(sprite_t *sprite) {
	if (!sprite || sprite->sprite_id < 0 || sprite->sprite_id >= sprite_registry_len)
		return NULL;

	int texture_id = sprite_registry[sprite->sprite_id].textures_ids[sprite->frame_idx];
	if (texture_id < 0 || texture_id >= texture_registry_len)
		return NULL;

	return texture_registry[texture_id].surface;
}

int get_sprite_id(const char *name) {
	for (int i = 0; i < sprite_registry_len; i++) {
		if (strcmp(sprite_registry[i].name, name) == 0) {
			return i;
		}
	}
	return -1;
}