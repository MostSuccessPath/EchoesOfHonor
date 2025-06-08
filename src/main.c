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

	hero.status.damage = 0;
	hero.status.endurance = 0;
	hero.status.health = 0;
	hero.status.speed = 0;
	hero.xp = 0;
	hero.level = 0;
	hero.avaiblePoints = 0;

	while(1) phase1(renderer, &hero);
	
	return 0;
}
