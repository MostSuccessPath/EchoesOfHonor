#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "structs.h"
#include "cJSON.h"
#include "cJSON_Utils.h"
#include "screens.h"
#include "readFiles.h"

#define NUM_MENUS 9
#define NUM_CHOICES 7

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
		            	
		            	if (i < 3) worldSelection(renderer);
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

void worldSelection(SDL_Renderer *renderer){
	tile_t *world;
	tile_t *slot;
	tile_t *heroTile;
	tile_t *slotHover;
	tile_t *textCharacter_tile = malloc(sizeof(tile_t) * 4);
	tile_t *textLVL_tile = malloc(sizeof(tile_t) * 4);
	tile_t *textPhase_tile = malloc(sizeof(tile_t) * 4);
	int color[] = {35, 59, 60};
	int i, mouseX, mouseY, mouseXHover, mouseYHover;
	
	world = loadImage(1, "./assets/images/WorldSelection/Worlds1.png", renderer);
	defineSize(1, world, 0, 0);
	
	heroTile = loadImage(3, "./assets/images/WorldSelection/Hero1.png", renderer);
	defineSize(3, heroTile, world->image.w, world->image.h);
	
	//Criar Slot:
	
	slot = loadImage(1, "./assets/images/WorldSelection/Slot1.png", renderer);
	
	slot->image.w = (world->image.w * 0.309375);
	slot->image.h = (world->image.h * 0.388888);
	
	slot->image.x = (world->image.w * 0.1552083);
	slot->image.y = (world->image.h * 0.183333);
	
	slotHover = loadImage(1, "./assets/images/WorldSelection/SlotHover1.png", renderer);
	
	SDL_Rect slotRect[4] = {
		{slot->image.x, slot->image.y, slot->image.w, slot->image.h},
		{world->image.w * 0.5328125, slot->image.y, slot->image.w, slot->image.h},
		{slot->image.x, world->image.h * 0.6018518, slot->image.w, slot->image.h},
		{world->image.w * 0.5328125, world->image.h * 0.6018518, slot->image.w, slot->image.h}
	};
	
	//Criar textos:
	
	worldSave_t *save = malloc(sizeof(worldSave_t) * 4);
	
	for(i = 0; i < 4; i++){
		char temp[50];
		
		snprintf(temp, 49, "./maps/Map%d.json", (i + 1));
		
		char *jsonText = readJson(temp);
	
		if(jsonText != NULL){
			
			collectJson(jsonText, &(save[i]));
			save[i].empty = 0;		
		}else{
			 save[i].empty = 1;
		}
	}
	
	int fontSize = 25 * (1920/world->image.w);
	
	for(i = 0; i < 4; i++){
		if(save[i].empty == 1) continue;

		createText("./fonts/PressStart2P.ttf", fontSize, color, renderer, save[i].characterStr, &(textCharacter_tile[i]));
		
		char temp[101];
		snprintf(temp, 100 ,"LVL: %d", save[i].hero.level);
		
		createText("./fonts/PressStart2P.ttf", fontSize, color, renderer, temp, &(textLVL_tile[i]));
		
		char temp2[101];
		snprintf(temp2, 100 , "Phase: %d", save[i].phase);
		
		createText("./fonts/PressStart2P.ttf", fontSize, color, renderer, temp2, &(textPhase_tile[i]));
	}
	
	SDL_Rect textCharacter[4] = {
		{(slotRect[0].x * 1.87248) , (slotRect[0].y * 1.4848484848), textCharacter_tile[0].image.w, textCharacter_tile[0].image.h},
		{(slotRect[1].x * 1.25415) , (slotRect[0].y * 1.4848484848), textCharacter_tile[1].image.w, textCharacter_tile[1].image.h},
		{(slotRect[2].x * 1.87248) , (slotRect[2].y * 1.1490683229), textCharacter_tile[2].image.w, textCharacter_tile[2].image.h},
		{(slotRect[3].x * 1.25415) , (slotRect[3].y * 1.1490683229), textCharacter_tile[3].image.w, textCharacter_tile[3].image.h}
	};
	
	SDL_Rect textLVL[4] = {
		{(slotRect[0].x * 1.87248) , (slotRect[0].y * 1.92424242424), textLVL_tile[0].image.w, textLVL_tile[0].image.h},
		{(slotRect[1].x * 1.25415) , (slotRect[1].y * 1.92424242424), textLVL_tile[1].image.w, textLVL_tile[1].image.h},
		{(slotRect[2].x * 1.87248) , (slotRect[2].y * 1.28416149068), textLVL_tile[2].image.w, textLVL_tile[2].image.h},
		{(slotRect[3].x * 1.25415) , (slotRect[3].y * 1.28416149068), textLVL_tile[3].image.w, textLVL_tile[3].image.h}
	};
	
	SDL_Rect textPhase[4] = {
		{(slotRect[0].x * 1.87248) , (slotRect[0].y * 2.35858585), textPhase_tile[0].image.w, textPhase_tile[0].image.h},
		{(slotRect[1].x * 1.25415) , (slotRect[1].y * 2.35858585), textPhase_tile[1].image.w, textPhase_tile[1].image.h},
		{(slotRect[2].x * 1.87248) , (slotRect[2].y * 1.41770186), textPhase_tile[2].image.w, textPhase_tile[2].image.h},
		{(slotRect[3].x * 1.25415) , (slotRect[3].y * 1.41770186), textPhase_tile[3].image.w, textPhase_tile[3].image.h}
	};
	
	SDL_Event event;
	
	while(1){
		SDL_GetMouseState(&mouseXHover, &mouseYHover);
		
		SDL_RenderClear(renderer);
		
		SDL_RenderCopy(renderer, world[0].texture, NULL, NULL);
		
		for(i = 0; i < 4; i++){
			
			if(save[i].empty == 1){
				
				if ((slotRect[i].x <= mouseXHover) && ((slotRect[i].x + slotRect[i].w) > mouseXHover) &&
				(slotRect[i].y <= mouseYHover) && ((slotRect[i].y + slotRect[i].h) > mouseYHover))
				SDL_RenderCopy(renderer, slotHover->texture, NULL, &slotRect[i]);
				continue;
			} 
			
			SDL_RenderCopy(renderer, slot->texture, NULL, &slotRect[i]);
			
			SDL_RenderCopy(renderer, textCharacter_tile[i].texture, NULL, &textCharacter[i]);
		
			SDL_RenderCopy(renderer, textLVL_tile[i].texture, NULL, &textLVL[i]);
			
			SDL_RenderCopy(renderer, textPhase_tile[i].texture, NULL, &textPhase[i]);
			
			SDL_RenderCopy(renderer, heroTile[save[i].hero.disposition.character].texture, NULL, &slotRect[i]);
			
			if ((slotRect[i].x <= mouseXHover) && ((slotRect[i].x + slotRect[i].w) > mouseXHover) &&
				(slotRect[i].y <= mouseYHover) && ((slotRect[i].y + slotRect[i].h) > mouseYHover)) 
				SDL_RenderCopy(renderer, slotHover->texture, NULL, &slotRect[i]);
		}
		
		SDL_RenderPresent(renderer);
		
		while(SDL_PollEvent(&event)){
			if(event.type == SDL_QUIT){
				exit(1);
			}else if(event.type == SDL_MOUSEBUTTONDOWN){
				
				mouseX = event.button.x;
				mouseY = event.button.y;
				
				for(i = 0; i < 4; i++){
					
					if ((slotRect[i].x <= mouseX) && ((slotRect[i].x + slotRect[i].w) > mouseX) &&
						(slotRect[i].y <= mouseY) && ((slotRect[i].y + slotRect[i].h) > mouseY)){
							
						if(save[i].empty == 1){
							// chamar a escolha de personagem
							chooseCharacter(renderer, i+1, &(save[i]));	
						}else{
							
							phase1(renderer, &save->hero);
						}
							
					}
					
				}
				
			}
		}
		
		SDL_Delay(16);
	}
}

void chooseCharacter(SDL_Renderer *renderer, int map, worldSave_t *save) {
	int i = 1;
	SDL_Event event;
	tile_t *choices;
	float time1, time2;
	int secs, tempSecs = 0;
	FILE *arch;
	int select = 0;
	
	char mapPath[50];
	
	snprintf(mapPath, 49 , "./maps/Map%d.json", map);
	
	arch = fopen(mapPath, "w");
	if(!arch){
		printf("Erro ao abrir arquivo");
		exit(1);
	}
	
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
		            		
		            		fprintf(arch, "{\n\"phase\": 1,\n\"health\": 28,\n\"speed\": 34,\n\"endurance\": 21,\n\"damage\": 32,\n\"xp\": 0,\n\"level\": 1,\n\"character\": \"Samurai\"\n}");
		            		fclose(arch);
							select = 1;	
						}
						
		            	else if (i < 5) {
		            		
		            		fprintf(arch, "{\n\"phase\": 1,\n\"health\": 15,\n\"speed\": 26,\n\"endurance\": 9,\n\"damage\": 45,\n\"xp\": 0,\n\"level\": 1,\n\"character\": \"Wizard\"\n}");
		            		fclose(arch);
							select = 1;	
						}
						
		            	else if (i < 7) {
		            		
		            		fprintf(arch, "{\n\"phase\": 1,\n\"health\": 41,\n\"speed\": 29,\n\"endurance\": 12,\n\"damage\": 25,\n\"xp\": 0,\n\"level\": 1,\n\"character\": \"Archer\"\n}");
		            		fclose(arch);
							select = 1;	
						}
						
						if(select == 1){
							char temp[50];
		
							snprintf(temp, 49, "./maps/Map%d.json", map);
							
							char *jsonText = readJson(temp);
							
							collectJson(jsonText, save);
							
							phase1(renderer, save->hero);
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

void readMap(map_t *map, char layersPath[][50]){
	int i, j, k;
	
	map->map = malloc(sizeof(int**) * map->layers);
	
	for(i = 0; i < map->layers; i++) map->map[i] = malloc(sizeof(int*) * map->h);
	
	for(i = 0; i < map->layers; i++){
		
		for(j = 0; j < map->h; j++){
			
			map->map[i][j] = malloc(sizeof(int*) * map->w);	
		}
	}
	
	FILE **f = malloc(sizeof(FILE *) * map->layers);
	
	for(i = 0; i < map->layers; i++){
		f[i] = fopen(layersPath[i], "r");
		if(!f[i]){
			printf("Erro ao abrir arquivo!");
			exit(1);
		}
		
	}
	
	for(i = 0; i < map->layers; i++){
		
		for(j = 0; j < map->h; j++){
			
			for(k = 0; k < map->w; k++){
				fscanf(f[i], " %d, ", &(map->map[i][j][k]));
			}
		}
	}
	
	for(i = 0; i < map->layers; i++) fclose(f[i]);
}



void printMap(int numTiles, int columns, int lines,  char *mapPathImage, SDL_Renderer *renderer, hero_t *hero, spawn_t *spawns, int spawnsCount, char *enemyPath){
	int i,j, k = 0;
	static tile_t *map, *heroTile;
	static map_t mapPhase1;
	mapPhase1.w = 234;
	mapPhase1.h = 248;
	mapPhase1.layers = 4;
	static SDL_Rect position, heroRect, heroCut;
	static SDL_Rect empty = {7 * 64, 5, TILE_SIZE, TILE_SIZE};
	static int oneTime = 0;
	static SDL_Rect *mapTiles;
	static tile_t *enemiesTexture;
	static tile_t *barrierTexture;
	
	
//	int numTiles, int columns, int lines,  char *mapPathImage, SDL_Renderer *renderer
	if(oneTime == 0){
		
		barrierTexture = loadImage(1, "./assets/images/Map1Elements/Barrier1.png", renderer);
		
		map = loadImage(1, mapPathImage, renderer);
		defineSize(1, map, 0, 0);
		
		mapTiles = malloc(sizeof(SDL_Rect) * numTiles);
		
		for(i = 0; i < lines; i++){
			for(j = 0; j < columns; j++){
			
				if(k == numTiles) break;
			
				mapTiles[k].x = j * 16;
				mapTiles[k].y = i * 16;
				mapTiles[k].w = 16;
				mapTiles[k].h = 16;
				k++;
			}
			if(k == numTiles) break;
		}
		
		char layers[4][50] = {
			{"./phases/layers1/piso.txt"},
			{"./phases/layers1/parede_vertical.txt"},
			{"./phases/layers1/parede_horizontal.txt"},
			{"./phases/layers1/decoracoes.txt"},
		};
	
		readMap(&mapPhase1, layers);
		
		position.w = TILE_SIZE;
		position.h = TILE_SIZE;
		
		heroRect.w = TILE_SIZE * 1.5;
		heroRect.h = TILE_SIZE * 1.5;
		
		char heroPath[100];
		
		if(hero->disposition.character == 0){
			strcpy(heroPath, "./assets/sprites/samurai/samurai_hero/walk1.png");
		}
		
		heroTile = loadImage(1, heroPath, renderer);
	
		
		oneTime = 1;
		
		enemiesTexture = loadImage(4, enemyPath, renderer);
	}
	
	int camX = (hero->image.x - (map->image.w/2));
	int camY = (hero->image.y - (map->image.h/2));

	for(i = 0; i < mapPhase1.layers; i++){
		
		for(j = 0; j < mapPhase1.h; j++){
				
			for(k = 0; k < mapPhase1.w; k++){
					
				position.x = ((k * TILE_SIZE) - camX);
				position.y = ((j * TILE_SIZE) - camY);
					
				if(mapPhase1.map[i][j][k] == -1){
					SDL_RenderCopy(renderer, map->texture, &empty, &position);
					continue;
				}
					
				SDL_RenderCopy(renderer, map->texture, &mapTiles[mapPhase1.map[i][j][k]], &position);
				
			}
		}
	}
	
	SDL_Rect enemy;
	SDL_Rect enemyCut;
	enemyCut.w = 64;
	enemyCut.h = 64;
	
	for(i = 0; i < spawnsCount; i++){
		for(j = 0; j < spawns[i].enemiesCount; j++){
			
			enemy.x =  spawns[i].enemies[j].image.x - camX;
			enemy.y = spawns[i].enemies[j].image.y - camY;
			enemy.w = 64;
			enemy.h = 64;
			
			enemyCut.x = (spawns[i].enemies[j].disposition.walking * 64);
			enemyCut.y = (spawns[i].enemies[j].disposition.orientation * 64);
			
			SDL_RenderCopy(renderer, enemiesTexture[spawns[i].enemies[j].disposition.character].texture , &enemyCut, &enemy);
		}
	}
	
	SDL_Rect barrier;
	barrier.w = TILE_SIZE;
	barrier.h = TILE_SIZE;
	
	for(i = 0; i < spawnsCount; i++){
		if(spawns[i].activated == 0) continue;
		
		for(j = 0; j < spawns[i].barriersCount; j++){
			
			barrier.x = spawns[i].barrier[j].x - camX;
			barrier.y = spawns[i].barrier[j].y - camY;
			
			SDL_RenderCopy(renderer, barrierTexture->texture, NULL, &barrier);
		}
	}
	
	heroRect.x = ((map->image.w/2) - (TILE_SIZE/2));
	heroRect.y = ((map->image.h/2) - (TILE_SIZE/2));

	heroCut.x = 32 + (hero->disposition.walking * 128);
	heroCut.y = 32 + (hero->disposition.orientation * 128);
	heroCut.w = 64;
	heroCut.h = 64;
		
	SDL_RenderCopy(renderer, heroTile->texture, &heroCut, &heroRect);

}

void statusScreen(SDL_Renderer *renderer, hero_t *hero, SDL_Event event) {
	
	static tile_t *status;
 	tile_t *numbers = malloc(sizeof(tile_t) * 7);
	tile_t *plus = malloc(sizeof(tile_t) * 4);
	tile_t *plusHover = malloc(sizeof(tile_t) * 4);
	tile_t *minus = malloc(sizeof(tile_t) * 4);
	tile_t *minusHover = malloc(sizeof(tile_t) * 4);
	tile_t *world = malloc(sizeof(tile_t));
	static int firstTime = 0;
	static activated = 0;
		
	int color[] = {35, 59, 60};
	int colorHover[] = {255,215,0};
	int i, mouseX, mouseY, mouseXHover, mouseYHover;
	
	/*numbers[0] = Health;
	  numbers[1] = Damage
	  numbers[2] = Endurance
	  numbers[3] = Speed
	  numbers[4] = Avaible Points
	  numbers[5] = Level
	  numbers[6] = XP
	  */

	defineSize(1, world, 0, 0);
	
	int fontSize = 35 * (1920/world->image.w);
	
	char textStatus[10];
	
	for(i = 0; i < 7; i++){
		
		if(i == 0) snprintf(textStatus, 9, "%.0f", hero->status.health);
		else if (i == 1) snprintf(textStatus, 9, "%.0f", hero->status.damage);
		else if (i == 2) snprintf(textStatus, 9, "%.0f", hero->status.endurance);
		else if (i == 3) snprintf(textStatus, 9, "%.0f", hero->status.speed);
		else if (i == 4) snprintf(textStatus, 9, "%d", hero->avaiblePoints);
		else if (i == 5) snprintf(textStatus, 9, "%d", hero->level);
		else snprintf(textStatus, 9, "%d", hero->xp);
		
		createText("./fonts/PressStart2P.ttf", fontSize, color, renderer, textStatus, &(numbers[i]));
	}
	
	char textOperands[2];
	
	fontSize = 50;
	
	for (i = 0; i < 4; i++) {
		
		strcpy(textOperands, "+");
		
		createText("./fonts/PressStart2P.ttf", fontSize, color, renderer, textOperands, &(plus[i]));
		createText("./fonts/PressStart2P.ttf", fontSize, colorHover, renderer, textOperands, &(plusHover[i]));
		
		strcpy(textOperands, "-");
		
		createText("./fonts/PressStart2P.ttf", fontSize, color, renderer, textOperands, &(minus[i]));
		createText("./fonts/PressStart2P.ttf", fontSize, colorHover, renderer, textOperands, &(minusHover[i]));
		
	}
	
	if(hero->status.health < 10) numbers[0].image.x = (world->image.w * 0.6171458333333333);
	else if (hero->status.health >= 10 && hero->status.health < 100) numbers[0].image.x = (world->image.w * 0.6071458333333333);
	else numbers[0].image.x = (world->image.w * 0.5971458333333333);
	numbers[0].image.y = (world->image.h * 0.45999333333333333);
	
	if(hero->status.damage < 10) numbers[1].image.x = (world->image.w * 0.6171458333333333);
	else if (hero->status.damage >= 10 && hero->status.damage < 100) numbers[1].image.x = (world->image.w * 0.6071458333333333);
	else numbers[1].image.x = (world->image.w * 0.5971458333333333);
	
	if(hero->status.endurance < 10) numbers[2].image.x = (world->image.w * 0.6171458333333333);
	else if (hero->status.endurance >= 10 && hero->status.endurance < 100) numbers[2].image.x = (world->image.w * 0.6071458333333333);
	else numbers[2].image.x = (world->image.w * 0.5971458333333333);
	
	if(hero->status.speed < 10) numbers[3].image.x = (world->image.w * 0.6171458333333333);
	else if (hero->status.speed >= 10 && hero->status.speed < 100) numbers[3].image.x = (world->image.w * 0.6071458333333333);
	else numbers[3].image.x = (world->image.w * 0.5971458333333333);
	
	SDL_Rect numbersRect[7] = {
		
		{numbers[0].image.x, numbers[0].image.y, numbers[0].image.w, numbers[0].image.h},
		{numbers[1].image.x, world->image.h * 0.5575925925925926, numbers[1].image.w, numbers[1].image.h},
		{numbers[2].image.x, world->image.h * 0.6525925925925926, numbers[2].image.w, numbers[2].image.h},
		{numbers[3].image.x, world->image.h * 0.7475000000000000, numbers[3].image.w, numbers[3].image.h},
		{world->image.w * 0.6552083333333333, world->image.h * 0.8802037037037037, numbers[4].image.w, numbers[4].image.h},
		{world->image.w * 0.433125, world->image.h * 0.3085185185185185, numbers[5].image.w, numbers[5].image.h},
		{world->image.w * 0.6775, world->image.h * 0.3135185185185185, numbers[6].image.w, numbers[6].image.h},
	};
	
	plus[0].image.x = (world->image.w * 0.665);
	plus[0].image.y = (world->image.h * 0.45899333333333333);
	
	SDL_Rect plusRect[4] = {
	
		{plus[0].image.x, plus[0].image.y, plus[0].image.w, plus[0].image.h},
		{plus[0].image.x, world->image.h * 0.5537037037037037, plus[0].image.w, plus[0].image.h},
		{plus[0].image.x, world->image.h * 0.6458333333333333, plus[0].image.w, plus[0].image.h},
		{plus[0].image.x, world->image.h * 0.7457407407407407, plus[0].image.w, plus[0].image.h},
	};
	
	minus[0].image.x = (world->image.w * 0.5589583333333333);
	minus[0].image.y = (world->image.h * 0.45899333333333333);

	SDL_Rect minusRect[4] = {
	
		{minus[0].image.x, minus[0].image.y, minus[0].image.w, minus[0].image.h},
		{minus[0].image.x, world->image.h * 0.5537037037037037, minus[0].image.w, minus[0].image.h},
		{minus[0].image.x, world->image.h * 0.6458333333333333, minus[0].image.w, minus[0].image.h},
		{minus[0].image.x, world->image.h * 0.7457407407407407, minus[0].image.w, minus[0].image.h},
	};
	
		SDL_GetMouseState(&mouseXHover, &mouseYHover);
			
			if (firstTime == 0) status = loadImage(1, "./assets/images/StatusScreen/StatusScreen1.png", renderer);
 		
			SDL_RenderCopy(renderer, status->texture, NULL, NULL);
			for(i = 0; i < 7; i++) SDL_RenderCopy(renderer, numbers[i].texture, NULL, &numbersRect[i]);
			
			for(i = 0; i < 4; i++){
			
				if ((plusRect[i].x <= mouseXHover) && ((plusRect[i].x + plusRect[i].w) > mouseXHover) &&
				(plusRect[i].y <= mouseYHover) && ((plusRect[i].y + plusRect[i].h) > mouseYHover))
				SDL_RenderCopy(renderer, plusHover->texture, NULL, &plusRect[i]);
				else SDL_RenderCopy(renderer, plus[i].texture, NULL, &plusRect[i]);
				
				if ((minusRect[i].x <= mouseXHover) && ((minusRect[i].x + minusRect[i].w) > mouseXHover) &&
				(minusRect[i].y <= mouseYHover) && ((minusRect[i].y + minusRect[i].h) > mouseYHover)) 
				SDL_RenderCopy(renderer, minusHover->texture, NULL, &minusRect[i]);
				else SDL_RenderCopy(renderer, minus[i].texture, NULL, &minusRect[i]);
			} 
	
				if(event.type == SDL_MOUSEBUTTONDOWN && event.type != 0){
					
					mouseX = event.button.x;
					mouseY = event.button.y;
					
					for(i = 0; i < 4; i++){
					
						if ((plusRect[i].x <= mouseX) && ((plusRect[i].x + plusRect[i].w) > mouseX) &&
						(plusRect[i].y <= mouseY) && ((plusRect[i].y + plusRect[i].h) > mouseY)) {
							
							if (hero->avaiblePoints == 0) continue;
							
							if(i == 0 && hero->status.health < 100) {
								
								hero->status.health += 1;
								hero->avaiblePoints -= 1;
							} 
							else if (i == 1 && hero->status.damage < 100) {
								
								hero->status.damage += 1;
								hero->avaiblePoints -= 1;
							} 
							else if (i == 2 && hero->status.endurance < 100) {
								
								hero->status.endurance += 1;
								hero->avaiblePoints -= 1;
							} 
							else if (i == 3 && hero->status.speed < 100) {
								
								hero->status.speed += 1;
								hero->avaiblePoints -= 1;
							} 
						}
						
						
						if ((minusRect[i].x <= mouseX) && ((minusRect[i].x + minusRect[i].w) > mouseX) &&
						(minusRect[i].y <= mouseY) && ((minusRect[i].y + minusRect[i].h) > mouseY)) {
							
							
							if(i == 0 && hero->status.health > 0) {
								
								hero->status.health -= 1;
								hero->avaiblePoints += 1;
							}
							else if (i == 1 && hero->status.damage > 0) {
								
								hero->status.damage -= 1;
								hero->avaiblePoints += 1;
							} 
							
							else if (i == 2 && hero->status.endurance > 0) {
								
								hero->status.endurance -= 1;
								hero->avaiblePoints += 1;
							}  
							else if (i == 3 && hero->status.speed > 0) {
								
								hero->status.speed -= 1;
								hero->avaiblePoints += 1;
							} 
						}
					} 
				}	    
		
		firstTime = 1;
		free(numbers);
		free(plus);
		free(plusHover);
		free(minus);
		free(minusHover);
		free(world);
}
