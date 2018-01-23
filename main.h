#ifndef MAIN_H
#define MAIN_H
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include <stdbool.h>



typedef struct{
    float x, y; // Coordinates of object
    float tempX, tempY;
    float width, height; // size of character
    float dx, dy; // Current Velocity
    float maxdy; //maximum vertical velocity
    bool jumping;
    bool groundCollision; // Does the character currently touch the ground?
    char *name;
    int lives;
    SDL_Texture *texture; // The texture used for 
} Character;

typedef struct{
    int x, y; //coordinates
    int height, width; 
} StaticObject;

typedef struct{
    float scrollX; // determines the part of the stage the hero/ player sees
    int time;
    char *currentStage;
    SDL_Renderer *renderer;

    bool updateHud; // currently only updates lives

    // Objects
    Character hero;
    StaticObject spawnPoint[1];
    StaticObject platforms[10];

    // Images
    SDL_Texture *livesLabel;
    SDL_Texture *timeLabel;
    SDL_Texture *stageLabel;

    // Fonts
    TTF_Font *font;

} GameState;

bool processEvents(SDL_Window *window, GameState *game); // handles user input like closing the game, moving the hero etc.

void doRender(GameState *game); // draws everything onto the screen

bool isColliding(GameState *game, float vectorX, float vectorY, bool debug);

void detectCollision(GameState *game);

void respawn(GameState *game);

void setSpawnpoint(GameState *game);

#endif