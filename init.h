#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include <stdbool.h>
// prototypes, allow calling from outside of main

void initHud(GameState *game);

void drawHud(GameState *game);

void clearHud(GameState *game);
