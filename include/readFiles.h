#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "structs.h"

#ifndef READFILES_H
#define READFILES_H

char* readJson(char *path);

void collectJson(char *jsonString, worldSave_t *save);

#endif
