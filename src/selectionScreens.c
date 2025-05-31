#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "structs.h"
#include "cJSON.h"
#include "cJSON_Utils.h"

#define NUM_MENUS 9
#define NUM_CHOICES 7

int menuScreen(SDL_Renderer *renderer) {
	
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
		            	
		            	if (i < 3) /*singlePlayer()*/;
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

char* readJson(char *path){
	int i;
	FILE *f;
	f = fopen(path, "r");
	
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	if(size <= 10) return NULL;
	rewind(f);
	
	char *jsonText = malloc((size + 1));
	
	fread(jsonText, 1, size, f);
	
	for(i = 0; i < size; i++){
		if(jsonText[i] == '}'){
			i++;
			break;
		}
	}
	
	jsonText[i] = '\0';
	
	fclose(f);
	
	return jsonText;
}

void collectJson(char *jsonString, worldSave_t *save){

	cJSON *json = cJSON_Parse(jsonString);
	if(json == NULL){
		printf("Erro ao analisar JSON.\n");
		exit(0);
	}
	
	cJSON *character = cJSON_GetObjectItemCaseSensitive(json, "character");
	cJSON *level = cJSON_GetObjectItemCaseSensitive(json, "level");
	cJSON *phase = cJSON_GetObjectItemCaseSensitive(json, "phase");
	
	strcpy(save->characterStr, character->valuestring);
	
	if(!(strcmp("Samurai", character->valuestring))) save->hero.disposition.character = 0; 
	else if(!(strcmp("Archer", character->valuestring))) save->hero.disposition.character = 1;
	else save->hero.disposition.character = 2;
	
	save->hero.level = level->valueint;
	
	save->phase = phase->valueint;
	
	cJSON_Delete(json);
}

int worldSelection(SDL_Renderer *renderer){
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
							printf("o save %d esta vazio\n", i);	
						}else{
							// carregar dados
							printf("o save %d esta com dados\n", i);
						}
							
					}
					
				}
				
			}
		}
		
		SDL_Delay(16);
	}
	
	return 0;
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
