#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_opengl.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_main.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_ttf.h"
#include <stdbool.h>



typedef struct{
    float x, y; // Coordinates of object
    float dx, dy; // Current Velocity
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

    SDL_Renderer *renderer;

    // Objects
    Character hero;
    StaticObject ground[10]; 
    StaticObject platforms[40];

    // Images
    SDL_Texture *label;

    // Fonts
    TTF_Font *font;

} GameState;

bool processEvents(SDL_Window *window, GameState *game); // handles user input like closing the game, moving the hero etc.

void doRender(GameState *game); // draws everything onto the screen