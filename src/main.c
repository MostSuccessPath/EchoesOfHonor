#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "selectionScreens.h"
#include "SDLfunctions.h"

int main(int argc, char *argv[]) {
	
	SDL_Renderer *renderer;
	SDL_Window *window;
	
	initSDL(&window, &renderer);

	worldSelection(renderer);
	
	return 0;
}
