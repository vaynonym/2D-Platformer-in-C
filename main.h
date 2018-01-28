#ifndef MAIN_H
#define MAIN_H
#include <stdbool.h>

#define N_PLATFORMS 42
extern const int width; 
extern const int height;
extern const int levelWidth;


typedef struct{
    float x, y; // Coordinates of object
    float tempX, tempY;
    float width, height; // size of character
    float dx, dy; // Current Velocity
    float maxdy; //maximum vertical velocity
    bool jumping;
    bool onMoving; // hero stands on a moving platform
    bool groundCollision; // Does the character currently touch the ground?
    char *name;
    int lives, points;
    SDL_Texture *texture; // The texture used for 
    SDL_RendererFlip flip;
} Character;

typedef struct{
    int x, y; //coordinates
    int height, width; 
    bool deadly, collisionFree;
    bool moveRight;
} StaticObject;

typedef struct {
    bool visible, increasePoints, increaseLives;
    float x, y;
} Collectible;

typedef struct{
    float scrollX; // determines the part of the stage the hero/ player sees
    int time;
    char *currentStage;
    SDL_Renderer *renderer;

    bool updateHud; // currently only updates lives

    // Objects
    Character hero;
    StaticObject spawnPoint[1];
    StaticObject platforms[N_PLATFORMS]; //PLATFORMS
    Collectible healthItems[2];
    Collectible pointItems[2];

    // Images
    SDL_Texture *livesLabel;
    SDL_Texture *timeLabel;
    SDL_Texture *stageLabel;

    SDL_Texture *background;

    // Fonts
    TTF_Font *font;

} GameState;

bool processEvents(SDL_Window *window, GameState *game); // handles user input like closing the game, moving the hero etc.

void doRender(GameState *game); // draws everything onto the screen


#endif