#include "game.h"

static int display_text(char *text, int x, int y, uint8_t col[3], size_t font) {
	SDL_Color sdl_color = (SDL_Color){col[0], col[1], col[2], 0xff};
	SDL_Surface	*text_surface = NULL;
	SDL_Rect rect;

	text_surface = TTF_RenderUTF8_Solid(game_ctx->fonts[font], text, sdl_color);
	if (!text_surface)
		return 0;

	rect = (SDL_Rect){x, y, text_surface->w, text_surface->h};
	int width = text_surface->w;

	SDL_BlitSurface(text_surface, NULL, game_surface, &rect);
	SDL_FreeSurface(text_surface);

	return width;
}

static int display_emote(size_t emote, int x, int y) {
	
	char emote_name[512] = {0};
	sprintf(emote_name, "emote_%lu", emote);

	int id = game_texture_get_id(emote_name);
	if (id == -1)
		return 0;
	
	SDL_Surface *emote_text = texture_registry[id].surface;
	SDL_Rect rect = (SDL_Rect){x, y, emote_text->w, emote_text->h};

	SDL_BlitSurface(emote_text, NULL, game_surface, &rect);

	return emote_text->w;
}

static void draw_box(int x, int y, int width, int height, size_t style) {
	if (!style)
		return ;

	SDL_Rect rect = {x, y, width, height};

	SDL_FillRect(game_surface, &rect, 
		SDL_MapRGB(game_surface->format, 255, 0, 255)
	);
}

void display_dialogue(dialogue_info_t *dialogue, size_t len, size_t sublen, int width, int height, size_t style, int x_start, int y, size_t font) {
	int x = x_start;
	uint8_t col[3] = {0};

	if (font >= sizeof(game_ctx->fonts) / sizeof(game_ctx->fonts[0]))
		font = 0;

	if (sublen || (len > 0 && dialogue->text[len - 1].type ==  DIALOG_TEXT))
		len++;
	
	if (len >= dialogue->len)
		len = dialogue->len;

	draw_box(x_start, y, width, height, style);

	size_t i = 0;
	while (i < len) {
		dialogue_char_t *c = &dialogue->text[i];
		if (c->type == DIALOG_TEXT || c->type == DIALOG_VAR) {
			char *text = NULL;
			size_t text_len = 0;
			if (c->type == DIALOG_TEXT) {
				text = &dialogue->original[c->text[0]];
				text_len = c->text[1];
			}
			else {
				text = "(NULL)";
				if (c->var < game_ctx->env_len)
					text = game_ctx->env_vars[c->var];
				text_len = strlen(text);
			}
			if (i == len - 1)
				text_len = sublen;
			char old_c = text[text_len];
			text[text_len] = '\0';

			x += display_text(text, x, y, col, font);

			text[text_len] = old_c;	

		}
		else if (c->type == DIALOG_LINE) {
			y += game_ctx->fonts_height[font];
			x = x_start;
		}
		else if (c->type == DIALOG_EMOTE)
			x += display_emote(c->emote, x, y);
		else if (c->type == DIALOG_CR)
			col[0] = c->r;
		else if (c->type == DIALOG_CG)
			col[1] = c->r;
		else if (c->type == DIALOG_CB)
			col[2] = c->r;
		i++;
	}
}
