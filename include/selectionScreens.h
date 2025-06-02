#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"

#ifndef SELECTIONSCREENS
#define SELECTIONSCREENS

void menuScreen(SDL_Renderer *renderer);

void chooseCharacter(SDL_Renderer *renderer, int map, worldSave_t *save);

char* readJson(char *path);

void collectJson(char *jsonString, worldSave_t *save);

void worldSelection(SDL_Renderer *renderer);

void printMap(int numTiles, int columns, int lines,  char *mapPathImage, SDL_Renderer *renderer, hero_t *hero);

#endif
