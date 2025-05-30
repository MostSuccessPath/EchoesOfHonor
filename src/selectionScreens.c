#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"

#define NUM_MENUS 9
#define NUM_CHOICES 7

void singlePlayer (SDL_Renderer *renderer);

void menuScreen(SDL_Renderer *renderer) {
	
	int i = 1;
	SDL_Event event;
	tile_t *menus;
	float time1, time2;
	int secs, tempSecs = 0;
	
	time1 = clock();
	
	char path[100] = "./assets/images/Menus/Menu .png";
	
	menus = loadImage(NUM_MENUS, path, renderer);
	
	defineSize(NUM_MENUS, menus, 0, 0);
	
	while(1) {
	
		time2 = clock();
		
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, menus[i].texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		
		secs = (((time2 - time1)/CLOCKS_PER_SEC)*10);
		
		while(SDL_PollEvent(&event)) {
		
		    if (event.type == SDL_QUIT) {
		    	
		        exit(0);
		        
		    } else if (event.type == SDL_KEYDOWN) {
		    	
		        switch (event.key.keysym.sym) {
		        	
		            case SDLK_w:
		            	
		            	if (i % 2 == 0){
		            		
		            		i--;
		            		i -= 2;
		            		
						} else{
							
							i -=2;
						}
		            	
		            	if (i < 1) i = 1;
		                break;
		                
		            case SDLK_s:
		            	
		            	if(i % 2 == 0){
		            		
		            		i--;
		            		i += 2;
		            		
						} else{
							i +=2;
						}
		            	
		            	if (i > 8) i = 8;
		                break;
		                
		            case SDLK_RETURN:
		            	
		            	if (i < 3) singlePlayer(renderer);
		            	else if (i < 5) /*multiPlayer()*/;
		            	else if (i < 7) /*settings()*/;
		            	else exit(1);
		            	
		            	break;
		                
		            default:
		                printf("Outra tecla: %s\n", SDL_GetKeyName(event.key.keysym.sym));
				}
			}
		}
		
		if (secs%5 == 0 && tempSecs != secs){
			
			if(i%2 == 0)i--;
			else i++;
			tempSecs = secs;
		}
		
		SDL_Delay(16);
	}
}

void singlePlayer (SDL_Renderer *renderer) {
	
	int i = 1;
	SDL_Event event;
	tile_t *choices;
	float time1, time2;
	int secs, tempSecs = 0;
	FILE *arch;
	
	arch = fopen("./maps/Map1/Map1.json", "w");
	
	time1 = clock();
	
	char path[100] = "./assets/images/ChooseCharacter/chooseCharacter .png";
	
	choices = loadImage(NUM_CHOICES, path, renderer);
	
	defineSize(NUM_CHOICES, choices, 0, 0);
	
	while(1) {
	
		time2 = clock();
		
		SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, choices[i].texture, NULL, NULL);
		SDL_RenderPresent(renderer);
		
		secs = (((time2 - time1)/CLOCKS_PER_SEC)*10);
		
		while(SDL_PollEvent(&event)) {
		
		    if (event.type == SDL_QUIT) {
		    	
		        exit(0);
		        
		    } else if (event.type == SDL_KEYDOWN) {
		    	
		        switch (event.key.keysym.sym) {
		        	
		            case SDLK_a:
		            	
		            	if (i % 2 == 0){
		            		
		            		i--;
		            		i -= 2;
		            		
						} else{
							
							i -=2;
						}
		            	
		            	if (i < 1) i = 1;
		                break;
		                
		            case SDLK_d:
		            	
		            	if(i % 2 == 0){
		            		
		            		i--;
		            		i += 2;
		            		
						} else{
							i +=2;
						}
		            	
		            	if (i > 6) i = 6;
		                break;
		                
		            case SDLK_RETURN:
		            	
		            	if (i < 3) {
		            		
		            		fprintf(arch, "{\n\"Phase\": 1,\n\"Health\": 28,\n\"Speed\": 34,\n\"Endurance\": 21,\n\"Damage\": 32,\n\"Xp\": 0,\n\"Level\": 1,\n\"Character\": \"Samurai\"\n}");
		            		fclose(arch);
							exit(1);	
						}
						
		            	else if (i < 5) {
		            		
		            		fprintf(arch, "{\n\"Phase\": 1,\n\"Health\": 15,\n\"Speed\": 26,\n\"Endurance\": 9,\n\"Damage\": 45,\n\"Xp\": 0,\n\"Level\": 1,\n\"Character\": \"Wizard\"\n}");
		            		fclose(arch);
							exit(1);	
						}
						
		            	else if (i < 7) {
		            		
		            		fprintf(arch, "{\n\"Phase\": 1,\n\"Health\": 41,\n\"Speed\": 29,\n\"Endurance\": 12,\n\"Damage\": 25,\n\"Xp\": 0,\n\"Level\": 1,\n\"Character\": \"Archer\"\n}");
		            		fclose(arch);
							exit(1);	
						}
		            	break;
		                
		            default:
		                printf("Outra tecla: %s\n", SDL_GetKeyName(event.key.keysym.sym));
				}
			}
		}
		
		if (secs%5 == 0 && tempSecs != secs){
			
			if(i%2 == 0)i--;
			else i++;
			tempSecs = secs;
		}
		
		SDL_Delay(16);
	}
}
