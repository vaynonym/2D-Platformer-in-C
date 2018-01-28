#ifndef LOADGAME_H
#define LOADGAME_H
#include "main.h"



void loadGame(GameState *game, SDL_Window *gameWindow); // a function that calls all loading functions

void initGame(GameState *game, SDL_Window *gameWindow); // loads the fundamental parts of the game: renderer, character, etc.

SDL_Texture* loadTexture(GameState *game, char *path);

void loadPlatforms(GameState *game); // loads platforms

#endif

