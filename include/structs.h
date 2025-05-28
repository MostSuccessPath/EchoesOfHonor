#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
	
	float x;
	float y;
	int h;
	int w;
	
} image_t;

typedef struct {
	
	int collision;
	int id;
	image_t image;
	SDL_Texture *texture;
	
} tile_t;

typedef struct {
	
	float health;
	float speed;
	float endurance;
	float damage;
	
} status_t;

typedef struct {
	
	int character;	
	int orientation;
	int walking;
	
} disposition_t;

typedef struct {
	
	image_t image;
	status_t status;
	disposition_t disposition;
	int xp;
	int level;
	
} hero_t;

typedef struct {
	
	image_t image;
	status_t status;
	disposition_t disposition;
	int xp_drop;
	int iten_drop;
	
} enemy_t;

typedef struct {
	
	float w;
	float h;
	int **map;
	
} map_t;

#endif
