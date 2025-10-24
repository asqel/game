#include "game.h"

typedef struct {
	uint32_t *dialogue;
	int len;
	int x;
	int y;
	uint8_t r;
	uint8_t g;
	uint8_t b;
	const SDL_PixelFormat *format;
	uint8_t style;
} data_t;


static int display_text(data_t *data, int i) {
	int text_len = 0;
	int text_start = i;
	while (i < data->len && data->dialogue[i] >> 24 == 0) {
		text_len++;
		i++;
	}
	if (!text_len)
		return 0;
	
	char *text = malloc(text_len + 1);
	text[text_len] = '\0';
	for (int k = 0; k < text_len; k++)
		text[k] = data->dialogue[text_start + k] & 0xff;

	SDL_Color sdl_color = (SDL_Color){~data->r, ~data->g, ~data->b, 0xff};
	SDL_Surface	*text_surface = NULL;
	SDL_Rect rect;

	text_surface = TTF_RenderUTF8_Solid(game_ctx->fonts[0], text, sdl_color);

	rect = (SDL_Rect){data->x, data->y, text_surface->w, text_surface->h};
	if (data->style == 1)
		SDL_FillRect(game_surface, &rect,
			SDL_MapRGB(data->format, ~data->r, ~data->g, ~data->b));

	rect.y--;
	SDL_BlitSurface(text_surface, NULL, game_surface, &rect);
	SDL_FreeSurface(text_surface);
	
	sdl_color = (SDL_Color){data->r, data->g, data->b, 0xFF};
	text_surface = TTF_RenderUTF8_Solid(
			game_ctx->fonts[0],
			text,
			sdl_color
	);
	rect.y++;
	SDL_BlitSurface(text_surface, NULL, game_surface, &rect);
	SDL_FreeSurface(text_surface);	

	free(text);
	return text_len;
}

static int display_emote(data_t *data, int i) {
	
	char emote_name[64] = {0};
	sprintf(emote_name, "emote_%d", data->dialogue[i] & 0x00FFFFFF);
	
	int id = game_texture_get_id(emote_name);
	if (id == -1)
		return 1;
	
	SDL_Surface *emote = texture_registry[id].surface;
	SDL_Rect rect = (SDL_Rect){data->x, data->y, emote->w, emote->h};
	SDL_BlitSurface(emote, NULL, game_surface, &rect);
	data->x += emote->w;
	return 1;
}

static int handle_special(data_t *data, int i) {
	uint32_t type = data->dialogue[i] & 0xFF000000;
	uint32_t val = data->dialogue[i] & 0x00FFFFFF;
	switch (type) {
		case DIALOG_CR:
			data->r = val;
			break;
		case DIALOG_CG:
			data->g = val;
			break;
		case DIALOG_CB:
			data->b = val;
			break;
		case DIALOG_STYLE:
			data->style = val;
			break;
	}
	return 1;
}

void display_dialogue(uint32_t *dialogue, int len, int x, int y) {
	data_t data = (data_t){0};
	data.dialogue = dialogue;
	data.len = len;
	data.x = x;
	data.y = y;
	data.format = game_surface->format;
	data.style = 0;
	int i = 0;
	while (i < len) {
		if ((dialogue[i] & 0xFF000000) == DIALOG_CHAR)	
			i += display_text(&data, i);
		else if ((dialogue[i] & 0xFF000000) == DIALOG_EMOTE)
			i += display_emote(&data, i);
		else
			i += handle_special(&data, i);
	}
}
