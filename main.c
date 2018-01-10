#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_opengl.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_main.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_ttf.h"
#include "main.h"
#include "init.h"
#define GRAVITY 0.32f


// Resolution of the game
const int width = 1920; 
const int height = 1080;
const int levelWidth = 10000;

void initGame(GameState *game, SDL_Window *gameWindow){
    /*
    SDL_RENDERER_ACCELERATED  = the renderer uses hardware acceleration
    SDL_RENDERER_PRESENTVSYNC =  synchronized with the refresh rate of the monitor */
    game->renderer = SDL_CreateRenderer(gameWindow, // name of window
                                                -1, // index of rendering driver, -1 being the first that supports the requested flags
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // requested flags, explained above
                                                );
    // hero
    game->scrollX = 0;
    game->hero.x = 300;
    game->hero.y = 300;
    game->hero.width = 30.0f;
    game->hero.height = 30.0f;
    game->hero.dx = 0;
    game->hero.dy = 0;
    game->hero.maxdy = -10.0; //maxdy maybe as level of difficulty
    game->hero.jumping = false;
    game->hero.groundCollision = true;
    game->hero.lives = 3;
    game->hero.name = "Hero";

    // platforms
    game->platforms[0].height = 400;
    game->platforms[0].width = 800;
    game->platforms[0].x = 400;
    game->platforms[0].y = 880;

    game->platforms[1].height = 400;
    game->platforms[1].width = 600;
    game->platforms[1].x = 1200;
    game->platforms[1].y = 750;

    game->platforms[2].height = 100;
    game->platforms[2].width = 400;
    game->platforms[2].x = 2400;
    game->platforms[2].y = 700;

    game->platforms[3].height = 20;
    game->platforms[3].width = 60;
    game->platforms[3].x = 3000;
    game->platforms[3].y = 950;

    game->platforms[4].height = 20;
    game->platforms[4].width = 60;
    game->platforms[4].x = 3500;
    game->platforms[4].y = 950;

    game->platforms[5].height = 20;
    game->platforms[5].width = 60;
    game->platforms[5].x = 4000;
    game->platforms[5].y = 950;

    game->platforms[6].height = 60;
    game->platforms[6].width = 400;
    game->platforms[6].x = 4200;
    game->platforms[6].y = 560;

    game->platforms[7].height = 20;
    game->platforms[7].width = 60;
    game->platforms[7].x = 4800;
    game->platforms[7].y = 950;


    // loading fonts (currently OpenFont does not work for some reason)
    
    game->font = TTF_OpenFont("Assets\\Fonts\\comicsans.ttf", 16);
    
    if(!game->font){
        printf("Cannot find font file");
        SDL_Quit();
        exit(1);
    }
    game->livesLabel = NULL;
    game->timeLabel = NULL;
    initHud(game);
}

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
        if(game->hero.dx > -8){ // if the playerCharacter has not exceeded the maximum (negative for left) velocity, increase the current velocity
            game->hero.dx -= 1;
        }
    }
    if(state[SDL_SCANCODE_RIGHT]){
        if(game->hero.dx < 8){ // if the playerCharacter has not exceeded the maximum (positive for right) velocity, increase the current velocity
            game->hero.dx += 1;
        }
    }
    if(game->hero.groundCollision && state[SDL_SCANCODE_UP]){
        game->hero.groundCollision = false;
        game->hero.jumping = true;
    }//this permits jumping
    // NOTE: groundcollision has to be set on true when the hero touches the ground
    if(game->hero.maxdy < 0){ 
        if(state[SDL_SCANCODE_UP] && game->hero.jumping){ // The hero jumps when the user presses up
            game->hero.dy -= 1.8;
            game->hero.maxdy += 1.2; // NOTE: maxdy has to be set on normal when groundcollision becomes true
        }else{
            game->hero.jumping = false;
        }
    }
    return running;
}

void doRender(GameState *game){

    SDL_Rect heroRect = {game->hero.x + game->scrollX, game->hero.y, game->hero.width, game->hero.height};

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255); // sets the color for the renderer to draw in
    SDL_RenderClear(game->renderer); //draws the entire screen with the color set

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // set new color
    SDL_RenderFillRect(game->renderer, &heroRect); // draw the rectangle in new color

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    for(int i = 0; i < 10; i++){
        if(game->platforms[i].x + game->scrollX - game->platforms[i].width <= width){ // only draw platforms which are visible on the screen
            SDL_Rect platform = {game->platforms[i].x + game->scrollX, game->platforms[i].y, game->platforms[i].width, game->platforms[i].height};
            SDL_RenderFillRect(game->renderer, &platform);
            // Adding game->scrollX to each x-coordinate accomplishes the sidescrolling effect
        }
    }
    drawHud(game);
    // done drawing
    SDL_RenderPresent(game->renderer); // render onto screen
    game->time = game->time + 1;
}

void detectCollision(GameState *game){
    if(game->hero.y > height){ //Respawn
        game->hero.y = 120.0f;
        game->hero.dy = 0.0f;
    }

    for(int i = 0; i < 10; i++){


        float mx = game->hero.x, my = game->hero.y;
        float bx = game->platforms[i].x, by = game->platforms[i].y,
            bw = game->platforms[i].width, bh = game->platforms[i].height;

        if(my+game->hero.height > by && by+bh > bx+bw){
            if(mx < bx+bw && bx+bw < mx+game->hero.width){
                game->hero.x = bw+game->hero.width;
                mx = bx+bw;
                game->hero.dx = 0.0f;
            }
            else if(mx+game->hero.width > bx && mx < bx){
                game->hero.x = bx-game->hero.width;
                mx = bx-game->hero.width;
                game->hero.dx = 0.0f;
            }
        }

        if(mx+game->hero.width > bx && mx<bx+bw){
            if(my < by+bh && my > by){
                game->hero.y = by+bh;
                game->hero.dy = 0.0f;
            }
            else if(my+game->hero.height > by && my < by){
                game->hero.y = by-game->hero.height;
                game->hero.dy = 0.0f;
                game->hero.groundCollision = true;
                game->hero.maxdy = -10.0;
            }
        }
    }
}

int main(int argc, char* args[]){
    SDL_Init(SDL_INIT_VIDEO); 
    TTF_Init(); // Initialize Font System

    GameState game; // a structure in which we save all important information, makes giving a function the relevant arguments much easier
    
    // Creates a cenetered window for our game
    SDL_Window *gameWindow = SDL_CreateWindow("Platformer", //window name
                                              SDL_WINDOWPOS_CENTERED, // initial x position
                                              SDL_WINDOWPOS_CENTERED, // initial y position
                                              width, height, //each in pixels
                                              SDL_WINDOW_OPENGL | SDL_WINDOW_BORDERLESS //flags, see https://wiki.libsdl.org/SDL_CreateWindow for a list of the flags
                                              );
    
    initGame(&game, gameWindow);
    
    while(processEvents(gameWindow, &game)){
        
        if( !(game.hero.x < 0 && game.hero.dx < 0) && !(game.scrollX < -levelWidth && game.hero.dx > 0)){ // if the player is not trying to leave the screen
            game.hero.x += game.hero.dx; // adjust position of characters according to velocity
        }
        // Physics
        game.hero.dy += GRAVITY;
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if(game.hero.dx > 0 && !state[SDL_SCANCODE_RIGHT])
            game.hero.dx -= 2;  
        else if(game.hero.dx < 0 && !state[SDL_SCANCODE_LEFT])
            game.hero.dx += 2; 
        game.hero.y += game.hero.dy; //for now, the player can leave the screen vertically

        game.scrollX = -game.hero.x + width / 2; // the hero is always at the center of the screen (horizontally)
        if (game.scrollX > 0){
            game.scrollX = 0; // except when he walks further to the left than his spawn point
        }

        detectCollision(&game);

        doRender(&game);

    }
    // Clearing all ressources used. Mandatory in C.
    
    TTF_CloseFont(game.font);
    clearHud(&game);
    SDL_DestroyRenderer(game.renderer);
    SDL_DestroyWindow(gameWindow);


    TTF_Quit();
    SDL_Quit();

    return 0;
}