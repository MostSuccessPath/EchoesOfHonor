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
	int speed = hero->status.speed * 100;	
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

void spawnEnemys(int spawnsCount, spawn_t **spawns){
	
	map_t mapEnemies;
	int i, j, k = 0;
	mapEnemies.w = 234;
	mapEnemies.h = 248;
	mapEnemies.layers = 1;
	int enemiesCount = 0;
	int barriersCount = 0;
	int character;
	
	*spawns = malloc(sizeof(spawn_t) * spawnsCount);
	
	char layers[1][50] =
	{
	{"./phases/layers1/enemies.txt"},
	};
	
	readMap(&mapEnemies, layers);
	
	for(i = 0; i < mapEnemies.h; i++){
		for(j = 0; j < mapEnemies.w; j++){
			if(mapEnemies.map[0][i][j] == 2){
				(*spawns)[k].vectH = i;
				(*spawns)[k].vectW = j;
				k++;
			}
		}
	}
	
	for(i = 0; i < spawnsCount; i++){
		(*spawns)[i].activated = 0;
		enemiesCount = 0;
		barriersCount = 0;
		
		for(j = (*spawns)[i].vectH - 27; j < (*spawns)[i].vectH + 27; j++){
			for(k = (*spawns)[i].vectW - 27; k < (*spawns)[i].vectW + 27; k++){
				
				if(j < 0 || k < 0 || j >= mapEnemies.h || k >= mapEnemies.w) continue;
				
				if(mapEnemies.map[0][j][k] == 0 || 
					mapEnemies.map[0][j][k] == 3 || 
					mapEnemies.map[0][j][k] == 4) enemiesCount++;
					
				if(mapEnemies.map[0][j][k] == 1) barriersCount++;
				
			}
			
		}
		
		(*spawns)[i].enemies = malloc(sizeof(enemy_t) * (enemiesCount + 1)); // depois ver aqui :D pode olhar +1? 
		(*spawns)[i].barrier = malloc(sizeof(image_t) * (barriersCount + 1));
		(*spawns)[i].enemiesCount = enemiesCount;
		(*spawns)[i].barriersCount = barriersCount;
	}
	
	for(i = 0; i < spawnsCount; i++){
		enemiesCount = 0;
		barriersCount = 0;
		
		for(j = (*spawns)[i].vectH - 27; j < (*spawns)[i].vectH + 27; j++){
			for(k = (*spawns)[i].vectW - 27; k < (*spawns)[i].vectW + 27; k++){
				
				if(j < 0 || k < 0 || j >= mapEnemies.h || k >= mapEnemies.w) continue;
				
				(*spawns)[i].enemies[enemiesCount].disposition.walking = 0;
				(*spawns)[i].enemies[enemiesCount].disposition.orientation = 2;
				
				if(mapEnemies.map[0][j][k] == 0 || 
					mapEnemies.map[0][j][k] == 3 || 
					mapEnemies.map[0][j][k] == 4 || 
					mapEnemies.map[0][j][k] == 5){
				
					(*spawns)[i].enemies[enemiesCount].image.x = k * TILE_SIZE;
					(*spawns)[i].enemies[enemiesCount].image.y = j * TILE_SIZE;
					
					character = mapEnemies.map[0][j][k];
					if(character > 0) character -= 2;
					(*spawns)[i].enemies[enemiesCount].disposition.character = character;
					enemiesCount++;
				}
				
				if(mapEnemies.map[0][j][k] == 1){
					(*spawns)[i].barrier[barriersCount].x = k * TILE_SIZE;
					(*spawns)[i].barrier[barriersCount].y = j * TILE_SIZE;
					barriersCount++;
				}
			}
		}
	}
}

void activeSpawns(spawn_t *spawns, int spawnsCount, hero_t *hero){
	int i;
	
	int vectW_hero = hero->image.x / TILE_SIZE;
	int vectH_hero = hero->image.y / TILE_SIZE;
	
	for(i = 0; i < spawnsCount; i++){
		
		if((vectH_hero < (spawns[i].vectH+20) && vectH_hero > (spawns[i].vectH-20)) &&
			(vectW_hero < (spawns[i].vectW+20) && vectW_hero > (spawns[i].vectW-20))){
				spawns[i].activated = 1;
			}
	}
}
