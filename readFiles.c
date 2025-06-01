#include <stdio.h>
#include <stdlib.h>
#include "readFiles.h"
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
	
	strcpy(save->characterStr, character->valuestring);
	
	if(!(strcmp("Samurai", character->valuestring))) save->hero.disposition.character = 0; 
	else if(!(strcmp("Archer", character->valuestring))) save->hero.disposition.character = 1;
	else save->hero.disposition.character = 2;
	
	save->hero.level = level->valueint;
	
	save->phase = phase->valueint;
	
	cJSON_Delete(json);
}
