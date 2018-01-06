#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_opengl.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_main.h"
#include <stdbool.h>


typedef struct{
    float x, y; // Coordinates of object
    float dx, dy; // Current Velocity
    float maxdy; //maximum vertical velocity
    bool jumping;
    bool groundCollision; // Does the character currently touch the ground?
    char *name;
    SDL_Texture *texture; // The texture used for 
} Character;

typedef struct{
    int x, y; //coordinates
    int height, width; 
} StaticObject;

typedef struct{
    float scrollX; // determines the part of the stage the hero/ player sees

    StaticObject platforms[10];

    SDL_Renderer *renderer;
    Character hero;
    StaticObject ground[10]; 
} GameState;

bool processEvents(SDL_Window *window, GameState *game); // handles user input like closing the game, moving the hero etc.

void doRender(GameState *game); // draws everything onto the screen