#ifndef HUD_H
#define HUD_H
#include "main.h"



// prototypes, allow calling from outside of main

void initHud(GameState *game);

void drawHud(GameState *game);

void clearHud(GameState *game);

SDL_Texture* createTextureFromString(GameState *game, char* text, SDL_Color color);

#endif