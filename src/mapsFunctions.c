#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include "screens.h"
#include "actions.h"

void phase1(SDL_Renderer *renderer, hero_t *hero) {
	int win = 0;
	float time1, time2;
	int secs, tempSecs = 0;
	int canWalk = 1;
	spawn_t *spawns;
	int i;
	time1 = clock();
	SDL_Event event, eventMouse;
	int activated = 0;
	SDL_Keycode key;
	
	hero->image.x = 80 * 48;
	hero->image.y = 230 * 48;

	spawnEnemys(6, &spawns);
	
	hero->avaiblePoints = 1000;
	
	while(1){
		key = 0;
		eventMouse.type = 0;
		
		SDL_RenderClear(renderer);
		
		time2 = clock();
		
		secs = (((time2 - time1)/CLOCKS_PER_SEC)*10);
		
		printMap(35, 8, 5, "./phases/Phase1.png", renderer, hero, spawns, 6, "./assets/sprites/wizard/wizard_enemy/walk1.png");
	
		if (tempSecs != secs && canWalk != 1){
			tempSecs = secs;
			canWalk = 1;
		}
		
		activeSpawns(spawns, 6, hero);
		
		while(SDL_PollEvent(&event)){
			if (event.type == SDL_KEYDOWN){
				
				key = event.key.keysym.sym;
				
				switch(event.key.keysym.sym){
					case SDLK_i:
						activated = 1;
						break;
					case SDLK_ESCAPE:
						if(activated == 1){
							activated = 0;
							break;
						}
						exit(1);
				}
				
			}else if(event.type == SDL_MOUSEBUTTONDOWN){
				
				eventMouse.type = event.type;
				eventMouse.button.x = event.button.x;
				eventMouse.button.y = event.button.y; 
				
			}
		}
		
		characterMove(renderer, hero, &canWalk, &activated, key);
		
		if(activated == 1) statusScreen(renderer, hero, eventMouse);
		
		if((secs - 1.1) > tempSecs) hero->disposition.walking = 0;
		
		SDL_RenderPresent(renderer);
			
		SDL_Delay(16);
	}	
}
