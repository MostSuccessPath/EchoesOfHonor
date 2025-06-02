#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include "selectionScreens.h"
#include "actions.h"

void phase1(SDL_Renderer *renderer, hero_t *hero) {
	int win = 0;
	float time1, time2;
	int secs, tempSecs = 0;
	int canWalk = 1;
	
	time1 = clock();
	
	hero->image.x = 80 * 48;
	hero->image.y = 230 * 48;

	while(1){
		
		time2 = clock();
		
		secs = (((time2 - time1)/CLOCKS_PER_SEC)*10);
		
		printMap(29, 8, 4, "./phases/Phase1.png", renderer, hero);
		
		if (tempSecs != secs && canWalk != 1){
			
			tempSecs = secs;
			canWalk = 1;
		}
		
		if((secs - 1.1) > tempSecs) hero->disposition.walking = 0;
		
		characterMove(renderer, hero, &canWalk);
		
		SDL_Delay(16);
		
	}	
	
}
