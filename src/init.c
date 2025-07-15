#include "game.h"

int init_sdl() {
	 if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Fenetre resizable avec surface projetée",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          800, 600, SDL_WINDOW_RESIZABLE);

    if (!window) {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Surface de jeu (offscreen buffer)
    SDL_Surface* gameSurface = SDL_CreateRGBSurfaceWithFormat(0, GAME_WIDTH, GAME_HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);
    if (!gameSurface) {
        SDL_Log("Erreur création gameSurface: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Texture utilisée pour projeter la surface sur la fenêtre
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                             SDL_TEXTUREACCESS_STREAMING, GAME_WIDTH, GAME_HEIGHT);

    SDL_bool running = SDL_TRUE;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = SDL_FALSE;
        }

        // Exemple simple de rendu sur la surface
        SDL_FillRect(gameSurface, NULL, SDL_MapRGB(gameSurface->format, 0, 0, 64)); // fond bleu foncé
        SDL_Rect rect = {50, 50, 100, 100};
        SDL_FillRect(gameSurface, &rect, SDL_MapRGB(gameSurface->format, 255, 0, 0)); // carré rouge

        // Mettre à jour la texture avec le contenu de la surface
        SDL_UpdateTexture(texture, NULL, gameSurface->pixels, gameSurface->pitch);

        // Nettoyer l'écran
        SDL_RenderClear(renderer);

        // Obtenir la taille actuelle de la fenêtre
        int winW, winH;
        SDL_GetRendererOutputSize(renderer, &winW, &winH);

        // Calculer le rectangle de destination (plein écran, stretch)
        SDL_Rect dstRect = {0, 0, winW, winH};

        // Dessiner la texture sur l'écran (scalée)
        SDL_RenderCopy(renderer, texture, NULL, &dstRect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(gameSurface);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;	
}

void game_init(int argc, char **argv) {
	if (argc > 2) {
		PRINT_ERR("Error: %s takes at most 1 argument\n", argv[0]);
		exit(1);
	}
	if (argc == 2)
		game_dir = argv[1];
	if (!game_path_is_dir(game_dir)) {
		PRINT_ERR("Error: %s is not a valid directory\n", game_dir);
		exit(1);
	}

	init_sdl();

}

