#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"

#ifndef SDLFUNCTIONS_H
#define SDLFUNCTIONS_H

void initSDL(SDL_Window **window, SDL_Renderer **renderer);

tile_t* loadImage(int numImage, char *path, SDL_Renderer *renderer);

void defineSize(int numTile, tile_t *tileset, int w, int h);

void createText(char *fontPath, int fontSize, int *color, SDL_Renderer *renderer, char *text);

#endif
