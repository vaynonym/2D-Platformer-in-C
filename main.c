#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include "main.h"
#include "init.h"
#define GRAVITY 0.16f


// Resolution of the game
const int width = 800; 
const int height = 600;
const int levelWidth = 3000;

void initGame(GameState *game, SDL_Window *gameWindow){
    /*
    SDL_RENDERER_ACCELERATED  = the renderer uses hardware acceleration
    SDL_RENDERER_PRESENTVSYNC =  synchronized with the refresh rate of the monitor */
    game->renderer = SDL_CreateRenderer(gameWindow, // name of window
                                                -1, // index of rendering driver, -1 being the first that supports the "requested flags"
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC // requested flags, explained above
                                                );
    // hero
    game->scrollX = 0;
    game->hero.x = 120;
    game->hero.y = 120;
    game->hero.dx = 0;
    game->hero.dy = 0;
    game->hero.maxdy = -10.0; //maxdy maybe as level of difficulty
    game->hero.jumping = false;
    game->hero.groundCollision = true;
    game->hero.lives = 3;
    game->hero.name = "Hero";

    // platforms, currently generated randomly
    srand(time(NULL));
    for (int i = 0; i < 40; i++){
        game->platforms[i].height = 40;
        game->platforms[i].width = 200;
        game->platforms[i].x = rand() % levelWidth;
        game->platforms[i].y = game->platforms[i].height / 2 + (rand() % (height - game->platforms[i].height));
    }

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
        if(game->hero.dx > -6){ // if the playerCharacter has not exceeded the maximum (negative for left) velocity, increase the current velocity
            game->hero.dx -= 0.5;
        }
    }
    if(state[SDL_SCANCODE_RIGHT]){
        if(game->hero.dx < 6){ // if the playerCharacter has not exceeded the maximum (positive for right) velocity, increase the current velocity
            game->hero.dx += 0.5;
        }
    }
    if(game->hero.groundCollision && state[SDL_SCANCODE_UP]){
        game->hero.groundCollision = false;
        game->hero.jumping = true;
    }//this permits jumping
    // NOTE: groundcollision has to be set on true when the hero touches the ground
    if(game->hero.maxdy < 0){ 
        if(state[SDL_SCANCODE_UP] && game->hero.jumping){ // The hero jumps when the user presses up
            game->hero.dy -= 0.5;
            game->hero.maxdy += 0.5; // NOTE: maxdy has to be set on normal when groundcollision becomes true
        }else{
            game->hero.jumping = false;
        }
    }
    return running;
}

void doRender(GameState *game){

    SDL_Rect heroRect = {game->hero.x + game->scrollX, game->hero.y, 30, 30};

    SDL_SetRenderDrawColor(game->renderer, 0, 0, 255, 255); // sets the color for the renderer to draw in
    SDL_RenderClear(game->renderer); //draws the entire screen with the color set

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255); // set new color
    SDL_RenderFillRect(game->renderer, &heroRect); // draw the rectangle in new color

    SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
    for(int i = 0; i < 10; i++){
        if(game->platforms[i].x + game->scrollX - game->platforms[i].width <= 800){ // only draw platforms which are visible on the screen
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
        //SIZE OF HERO SHOULD BE STORED AS CONSTANT
        float mw = 30.0f, mh = 30.0f;

        float mx = game->hero.x, my = game->hero.y;
        float bx = game->platforms[i].x, by = game->platforms[i].y,
            bw = game->platforms[i].width, bh = game->platforms[i].height;

        if(my+mh > by && by+bh > bx+bw){
            if(mx < bx+bw && bx+bw < mx+mw){
                game->hero.x = bw+mw;
                mx = bx+bw;
                game->hero.dx = 0.0f;
            }
            else if(mx+mw > bx && mx < bx){
                game->hero.x = bx-mw;
                mx = bx-mw;
                game->hero.dx = 0.0f;
            }
        }

        if(mx+mw > bx && mx<bx+bw){
            if(my < by+bh && my > by){
                game->hero.y = by+bh;
                game->hero.dy = 0.0f;
            }
            else if(my+mh > by && my < by){
                game->hero.y = by-mh;
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
                                              SDL_WINDOW_OPENGL //flags, see https://wiki.libsdl.org/SDL_CreateWindow for a list of the flags
                                              );
    
    initGame(&game, gameWindow);
    
    while(processEvents(gameWindow, &game)){
        
        if( !(game.hero.x < 0 && game.hero.dx < 0) && !(game.scrollX < -levelWidth && game.hero.dx > 0)){ // if the player is not trying to leave the screen
            game.hero.x += game.hero.dx; // adjust position of characters according to velocity
        }
        game.hero.dy += GRAVITY;
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