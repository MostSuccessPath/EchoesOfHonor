#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "structs.h"
#include "cJSON.h"
#include "cJSON_Utils.h"

SDL_DisplayMode dm;

void initSDL(SDL_Window **window, SDL_Renderer **renderer) {
	
	pthread_mutex_init(&heroPositionMutex, NULL);
	
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		
        printf("Erro ao iniciar SDL: %s\n", SDL_GetError());
        exit(0);
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    	
        printf("Erro ao iniciar SDL_image: %s\n", IMG_GetError());
        exit(0);
    }
    
    if (TTF_Init() < 0) {
    	
	    printf("Erro ao inicializar SDL_ttf: %s\n", TTF_GetError());
		exit(0);
	}
    
    if (SDL_GetCurrentDisplayMode(0, &dm) != 0) {
    	
    	printf("Erro ao obter resolu��o da tela: %s\n", SDL_GetError());
    	exit(0);
	}

    *window = SDL_CreateWindow("Exemplo SDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, dm.w, dm.h, SDL_WINDOW_SHOWN);
    
    if (!(*window)) {
        printf("Erro ao criar janela: %s\n", SDL_GetError());
        exit(0);
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!(*renderer)) {
        printf("Erro ao criar renderizador: %s\n", SDL_GetError());
        exit(0);
    }
}

tile_t *loadImage(int numImage, char *path, SDL_Renderer *renderer){
	
	int i = 0, j = 0;
	tile_t *image = malloc (sizeof(tile_t) * numImage);
	char imagePath[100];
	int pathSize = strlen(path) - 5;
	
	strcpy(imagePath, path);
	
	SDL_Surface *surface;
	
	for (i = 0; i < numImage; i++) {
		
		imagePath[pathSize] = 49 + i;
		surface = IMG_Load(imagePath);
		
		if (!surface) {
			
        	printf("Erro ao carregar imagem %s: %s\n", imagePath, IMG_GetError());
    		exit(1);
		}
		
		image[i].texture = SDL_CreateTextureFromSurface(renderer, surface);
		
		SDL_FreeSurface(surface);
		
	    if (!image[i].texture) {
	    	
        	printf("Erro ao criar textura: %s\n", SDL_GetError());
        	exit(1);
	   }
	}
	
	return image;
}

void defineSize(int numTile, tile_t *tileset, int w, int h) {
	
	int i = 0;
	
	for (i = 0; i < numTile; i++) {
		
		if (w == 0) tileset[i].image.w = dm.w;
		else tileset[i].image.w = w;
		
		if (h == 0) tileset[i].image.h = dm.h;
		else tileset[i].image.h = h; 
	}
}

void createText(char *fontPath, int fontSize, int *color, SDL_Renderer *renderer, char *text, tile_t *textTile){
	
	TTF_Font* font = TTF_OpenFont(fontPath, fontSize);
    if (!font) {
        printf("Erro ao carregar fonte: %s\n", TTF_GetError());
        exit(0);
    }
    
    SDL_Color fontColor = { color[0], color[1], color[2] };
    
	SDL_Surface* surfaceText = TTF_RenderText_Blended(font, text, fontColor);
    
	SDL_Texture* textureText = SDL_CreateTextureFromSurface(renderer, surfaceText);
	
	textTile->image.w = surfaceText->w;
	textTile->image.h = surfaceText->h;
	textTile->texture = textureText;
	
    SDL_FreeSurface(surfaceText);
}
