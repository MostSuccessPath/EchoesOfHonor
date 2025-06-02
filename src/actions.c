#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include "actions.h"
#include "structs.h"

void characterMove(SDL_Renderer *renderer, hero_t *hero) {
	
	Uint32 lastTime = SDL_GetTicks();
	SDL_Event event;
	
	while(1) {
	
		Uint32 currentTime = SDL_GetTicks();
		
		float deltaTime = (currentTime - lastTime);
		
		lastTime = currentTime;
		
		while(SDL_PollEvent(&event)) {
		
		    if (event.type == SDL_QUIT) {
		    	
		        exit(0);
		        
		    } else if (event.type == SDL_KEYDOWN) {
		    	
		        switch (event.key.keysym.sym) {
		        	
		            case SDLK_a:
		            	
		            	hero->image.x -= hero->status.speed * deltaTime;
		            	
		                break;
		                
		            case SDLK_d:
		            	
		            	hero->image.x += hero->status.speed * deltaTime;
		            	
		                break;
		                
		            case SDLK_w:
		            	
		            	hero->image.y -= hero->status.speed * deltaTime;
		            	
		            	break;
		            	
		            case SDLK_s:
		            	
		            	hero->image.y += hero->status.speed * deltaTime;
		            	
		            	break; 	
				}
			}
		}
	}
}
