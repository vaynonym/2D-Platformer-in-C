#ifndef COLLECTIBLE_H
#define COLLECTIBLE_H
#include "main.h"
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"

void testForAllCollectibles(GameState *game);

void testForCollectibles(SDL_Rect rectHero, GameState *game, int max, Collectible *collectibles);

void resetCollectibles(GameState *game);

#endif