#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "screens.h"
#include "SDLfunctions.h"
#include "structs.h"

int main(int argc, char *argv[]){
	hero_t hero;
	SDL_Renderer *renderer;
	SDL_Window *window;
	
	initSDL(&window, &renderer);

	menuScreen(renderer);
	
	return 0;
}
