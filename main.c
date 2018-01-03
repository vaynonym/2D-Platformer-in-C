#include <stdio.h>
#include <stdlib.h>
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\Misc\include\SDL2\SDL.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_opengl.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_main.h"
#include <stdbool.h>
#include "main.h"

// Resolution of the game
const int width = 800; 
const int height = 600;

bool processEvents(SDL_Window *window, GameState *game){
    SDL_Event event;
    bool running = true;
    while(SDL_PollEvent(&event)){
        switch(event.type){
            case SDL_WINDOWEVENT_CLOSE:{
                if(window){
                    SDL_DestroyWindow(window);
                    window = NULL;
                    running = false;
                }
            }
            break;
            case SDL_KEYDOWN:{
                switch(event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        running = false;
                    break;
                }
            }
            break;
            case SDL_QUIT:
                running = false;
            break;
        }
    }

    //alternative, better way of getting which Key is pressed:
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    if(state[SDL_SCANCODE_LEFT]){
        if(game->hero.dx > -6){ // if the playerCharacter has not exceeded the maximum (negative for left) velocity, increase the current velocity
            game->hero.dx -= 0.5;
        }
    }
    if(state[SDL_SCANCODE_RIGHT]){
        if(game->hero.dx < 6){ // if the playerCharacter has not exceeded the maximum (positive for right) velocity, increase the current velocity
            game->hero.dx += 0.5;
        }
    }
    if(state[SDL_SCANCODE_UP]){ // The hero jumps when the user presses up
        if(!game->hero.groundCollision){
            game->hero.dy -= 0.5;
        }
    }
    return running;
}

void doRender(GameState *game){
    // SDL_Renderer *renderer = game->renderer;
    SDL_Rect characterRect = {game->hero.x, game->hero.y, 30, 30};

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255); // sets the color for the renderer to draw in
    SDL_RenderClear(game->renderer); //draws the entire screen with the color set

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // set new color
    SDL_RenderFillRect(game->renderer, &characterRect); // draw the rectangle in new color
    // done drawing
    SDL_RenderPresent(game->renderer); // render onto screen
}

int main(int argc, char* args[]){
    GameState game; // a structure in which we save all important information, makes giving a function the relevant arguments much easier
    SDL_Init(SDL_INIT_VIDEO); 
    // Creates a cenetered window for our game
    SDL_Window *gameWindow = SDL_CreateWindow("Hello World!", //window name
                                              SDL_WINDOWPOS_CENTERED, // initial x position
                                              SDL_WINDOWPOS_CENTERED, // initial y position
                                              width, height, //each in pixels
                                              SDL_WINDOW_OPENGL //flags, see https://wiki.libsdl.org/SDL_CreateWindow for a list of the flags
                                              );
    /*
    SDL_RENDERER_ACCELERATED  = the renderer uses hardware acceleration
    SDL_RENDERER_PRESENTVSYNC =  synchronized with the refresh rate of the monitor */
    game.renderer = SDL_CreateRenderer(gameWindow, // name of window
                                                -1, // index of rendering driver, -1 being the first that supports the "requested flags"
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // requested flags, explained above
                                                );
    
    game.hero.x = 120;
    game.hero.y = 120;
    game.hero.dx = 0;
    game.hero.dy = 0;
    while(processEvents(gameWindow, &game)){
        game.hero.x += game.hero.dx; // adjust position of characters according to velocity
        game.hero.y += game.hero.dy;
        doRender(&game);
    }
    // Clearing all ressources used. Mandatory in C.
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(gameWindow);
    SDL_Quit();
    printf("SDL quit.");
    

    return 0;
}