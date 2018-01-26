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
#include "loadGame.h"
#include "collisionDetection.h"
#include "collectible.h"
#define GRAVITY 0.32f


// Resolution of the game
const int width = 1920; 
const int height = 1080;
const int levelWidth = 12000;

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
    if(state[SDL_SCANCODE_A]){
        if(game->hero.dx > -6){ // if the playerCharacter has not exceeded the maximum (negative for left) velocity, increase the current velocity
            game->hero.dx -= 2;
        }
    }
    if(state[SDL_SCANCODE_D]){
        if(game->hero.dx < 6){ // if the playerCharacter has not exceeded the maximum (positive for right) velocity, increase the current velocity
            game->hero.dx += 2;
        }
    }
    if(game->hero.groundCollision && state[SDL_SCANCODE_SPACE]){
        game->hero.groundCollision = false;
        game->hero.jumping = true;
    }//this permits jumping
    // NOTE: groundcollision has to be set on true when the hero touches the ground
    if(game->hero.maxdy < 0){ 
        if(state[SDL_SCANCODE_SPACE] && game->hero.jumping){ // The hero jumps when the user presses up
            game->hero.dy -= 2;
            game->hero.maxdy += 1.8; // NOTE: maxdy has to be set on normal when groundcollision becomes true
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
    for(int i = 0; i < 42; i++){ //PLATFORMS
        if(game->platforms[i].x + game->scrollX - game->platforms[i].width <= width){ // only draw platforms which are visible on the screen
            SDL_Rect platform = {game->platforms[i].x + game->scrollX, game->platforms[i].y, game->platforms[i].width, game->platforms[i].height};
            SDL_RenderFillRect(game->renderer, &platform);
            // Adding game->scrollX to each x-coordinate accomplishes the sidescrolling effect
        }
    }

    SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);

    for(int i = 0; i < 2; i++){
        Collectible collectible = game->healthItems[i];
        if(collectible.visible){
            SDL_Rect rectCollectible = {collectible.x + game->scrollX, collectible.y, 50, 50};
            SDL_RenderFillRect(game->renderer, &rectCollectible);
        }
    }

    SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);

    for(int i = 0; i < 2; i++){
        Collectible collectible = game->pointItems[i];
        if(collectible.visible){
            SDL_Rect rectCollectible = {collectible.x + game->scrollX, collectible.y, 50, 50};
            SDL_RenderFillRect(game->renderer, &rectCollectible);
        }
    }

    drawHud(game);
    // done drawing
    SDL_RenderPresent(game->renderer); // render onto screen
    game->time = game->time + 1;
}

void movePlatform(GameState *game){
    if(game->platforms[9].x > 4550){
        game->platforms[9].moveRight = false;
    }else if(game->platforms[9].x < 3950){
        game->platforms[9].moveRight = true;
    }
    if(game->platforms[9].moveRight){
        game->platforms[9].x += 6;
    }else{
        game->platforms[9].x -= 6;
    }
    //moves paltform[9]
    if(game->platforms[12].y > 300 ){
        game->platforms[12].moveRight = false;
    }else if(game->platforms[12].y < 100){
        game->platforms[12].moveRight = true;
    }
    if(game->platforms[12].moveRight){
        game->platforms[12].y += 6;
    }else{
        game->platforms[12].y -= 6;
    }
    //moves platform[12]

    if(game->platforms[13].y > 950){
        game->platforms[13].moveRight = false;
    }else if(game->platforms[13].y < 750){
        game->platforms[13].moveRight = true;
    }
    if(game->platforms[13].moveRight){
        game->platforms[13].y += 6;
    }else{
        game->platforms[13].y -= 6;
    }
    //moves platform[13]

    if(7500 < game->platforms[20].x){
        game->platforms[20].moveRight = false;
    }else if(game->platforms[20].x < 6400){
        game->platforms[20].moveRight = true;                
    }
    if(game->platforms[20].moveRight){
        game->platforms[20].x += 6;
    }else{
        game->platforms[20].x -= 6;
    }
    //moves the platform[20]
}

void setSpawnpoint(GameState *game){
    if(game->hero.groundCollision && (game->hero.x > game->spawnPoint[0].x + 100)){
        game->spawnPoint[0].x = game->hero.x;
        game->spawnPoint[0].y = game->hero.y;
    }
}
// dynamischer spawnpoint

void respawn(GameState *game){
    game->hero.dx = 0;
    game->hero.dy = 0;
    game->hero.lives--;
    
    if(game->hero.lives == 0 ){ // gameover. Restarting game.
        game->hero.x = 400;
        game->hero.y = 300;
        game->spawnPoint[0].x = 400;
        game->spawnPoint[0].y = 300;
        game->hero.lives = 3;
        resetCollectibles(game);
    }
    else{ // normal respawn
        game->hero.x = game->spawnPoint[0].x;
        game->hero.y = game->spawnPoint[0].y;
    }
    game->updateHud = true; // update the now changed lives
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
    
    loadGame(&game, gameWindow);
    
    while(processEvents(gameWindow, &game)){
        
        game.hero.tempX = game.hero.x;
        game.hero.tempY = game.hero.y;

        if( !(game.hero.tempX < 0 && game.hero.dx < 0) && !(game.scrollX < -levelWidth && game.hero.dx > 0)){ // if the player is not trying to leave the screen
            game.hero.tempX += game.hero.dx; // adjust position of characters according to velocity
        }
        
        // Physics
        game.hero.dy += GRAVITY;
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        if(game.hero.dx > 0 && !state[SDL_SCANCODE_D])
            game.hero.dx -= 2;  
        else if(game.hero.dx < 0 && !state[SDL_SCANCODE_A])
            game.hero.dx += 2; 
        game.hero.tempY += game.hero.dy; //for now, the player can leave the screen vertically
        game.hero.tempX += game.hero.dx;
        detectCollision(&game);
        
        setSpawnpoint(&game);
        if(game.hero.y > 1000){
            respawn(&game);
        }

        game.scrollX = -game.hero.x + width / 2; // the hero is always at the center of the screen (horizontally)
        if (game.scrollX > 0){
            game.scrollX = 0; // except when he walks further to the left than his spawn point
        }

        movePlatform(&game);

        testForAllCollectibles(&game);

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