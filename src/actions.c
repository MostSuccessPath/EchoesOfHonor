#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "actions.h"
#include "structs.h"

void characterMove(SDL_Renderer *renderer, hero_t *hero, int *canWalk) {
	
	static float lastTime = 0;
	SDL_Event event;

	float currentTime = SDL_GetTicks();
	float deltaTime = (currentTime - lastTime) / 1000.0f;
	int speed = hero->status.speed * 10;	
	lastTime = currentTime;
	
	while(SDL_PollEvent(&event)) {
		
		if (event.type == SDL_QUIT) {
		    	
		    exit(0);
		        
		} else if (event.type == SDL_KEYDOWN) {
		    
		    int startOrientation = hero->disposition.orientation;
		    
		    switch (event.key.keysym.sym) {
		        	
		        case SDLK_a:
		            	
		           	hero->image.x -= speed * deltaTime;
		           	hero->disposition.orientation = 1;
		            	
		            break;
		                
		    	case SDLK_d:
		            	
		            hero->image.x += speed * deltaTime;
		            hero->disposition.orientation = 3;
		            	
		            break;
		                
		        case SDLK_w:
		            	
		            hero->image.y -= speed * deltaTime;
		            hero->disposition.orientation = 0;
		            
		            break;
		            	
		        case SDLK_s:
		            	
		            hero->image.y += speed * deltaTime;
		            hero->disposition.orientation = 2;
		            	
		            break; 	
			}
			
			if(*canWalk){
				if(startOrientation == hero->disposition.orientation){
				
					hero->disposition.walking += 1;
					if(hero->disposition.walking % 9 == 0) hero->disposition.walking = 0;
				
				}else{
					
					hero->disposition.walking = 0;
				}
				*canWalk = 0;
			}
		}
	}
}
