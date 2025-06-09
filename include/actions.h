#include "structs.h"

#ifndef ACTIONS_H
#define ACTIONS_H

void characterMove(SDL_Renderer *renderer, hero_t *hero, int *canWalk, int *activated, SDL_Keycode event);

void spawnEnemys(int spawnsCount, spawn_t **spawns);

void activeSpawns(spawn_t *spawns, int spawnsCount, hero_t *hero);

#endif
