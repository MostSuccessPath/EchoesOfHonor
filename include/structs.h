#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <pthread.h>
#include <semaphore.h>

#ifndef STRUCTS_H
#define STRUCTS_H

pthread_mutex_t heroPositionMutex, eventMutex;

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

typedef struct{
	
	hero_t hero;
	int phase;
	char characterStr[50];
	int empty;
	
}worldSave_t;

typedef struct {
	
	image_t image;
	status_t status;
	disposition_t disposition;
	int xp_drop;
	int iten_drop;
	
} enemy_t;

typedef struct {
	
	int w;
	int h;
	int layers;
	int ***map;
	
} map_t;

typedef struct{
	
	int numTiles;
	int columns; 
	int lines;  
	char *mapPathImage; 
	SDL_Renderer *renderer;
	hero_t hero;
	int event;
	
} phase1_t;

#endif
