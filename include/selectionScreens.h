#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"

#ifndef SELECTIONSCREENS_H
#define SELECTIONSCREENS_H

int menuScreen(SDL_Renderer *renderer);

void worldSelection(SDL_Renderer *renderer);

void chooseCharacter(SDL_Renderer *renderer, int map);

#endif
