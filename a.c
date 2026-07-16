#include <SDL2/SDL.h>
#include <stdio.h>

int main(void)
{
    int n = SDL_GetNumVideoDrivers();

    for (int i = 0; i < n; i++)
        printf("%s\n", SDL_GetVideoDriver(i));
	
	char *s = "aa";

    return 0;
}
