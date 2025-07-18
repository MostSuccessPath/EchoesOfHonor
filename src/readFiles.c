#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "cJSON.h"
#include "cJSON_Utils.h"

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
	cJSON *health = cJSON_GetObjectItemCaseSensitive(json, "health");
	cJSON *speed = cJSON_GetObjectItemCaseSensitive(json, "speed");
	cJSON *endurance = cJSON_GetObjectItemCaseSensitive(json, "endurance");
	cJSON *damage = cJSON_GetObjectItemCaseSensitive(json, "damage");
	cJSON *xp = cJSON_GetObjectItemCaseSensitive(json, "xp");
	
	strcpy(save->characterStr, character->valuestring);
	
	if(!(strcmp("Samurai", character->valuestring))) save->hero.disposition.character = 0; 
	else if(!(strcmp("Archer", character->valuestring))) save->hero.disposition.character = 1;
	else save->hero.disposition.character = 2;
	
	save->hero.level = level->valueint;
	save->hero.status.health = health->valueint;
	save->hero.status.speed = speed->valueint;
	save->hero.status.endurance = endurance->valueint;
	save->hero.status.damage = damage->valueint;
	save->hero.xp = xp->valueint;
	
	save->phase = phase->valueint;
	
	cJSON_Delete(json);
}
