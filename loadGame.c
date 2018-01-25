#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include "init.h"
#include "loadGame.h"
#include "main.h"
#include "collectible.h"

void loadGame(GameState *game, SDL_Window *gameWindow){
    initGame(game, gameWindow);
    loadPlatforms(game);
}

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
    game->hero.x = 400;
    game->hero.y = 300;
    game->hero.width = 30.0f;
    game->hero.height = 30.0f;
    game->hero.dx = 0;
    game->hero.dy = 0;
    game->hero.maxdy = -10.0; //maxdy maybe as level of difficulty
    game->hero.jumping = false;
    game->hero.groundCollision = false;
    game->hero.lives = 3;
    game->hero.points = 0;
    game->hero.name = "Hero";

    // spawnpoint
    game->spawnPoint[0].x = 300;
    game->spawnPoint[0].y = 300;
    

    // loading fonts (currently OpenFont does not work for some reason)
    
    game->font = TTF_OpenFont("Assets/Fonts/comicsans.ttf", 16);
    
    if(!game->font){
        printf("Cannot find font file");
        SDL_Quit();
        exit(1);
    }
    game->livesLabel = NULL;
    game->timeLabel = NULL;
    initHud(game);
}

void loadPlatforms(GameState *game){
    // platforms
    game->platforms[0].height = 400;
    game->platforms[0].width = 400; //800
    game->platforms[0].x = 400;
    game->platforms[0].y = 880;

    game->platforms[1].height = 400;
    game->platforms[1].width = 600;
    game->platforms[1].x = 1200;
    game->platforms[1].y = 750;

    game->platforms[2].height = 100;
    game->platforms[2].width = 400;
    game->platforms[2].x = 2400;
    game->platforms[2].y = 900;

    game->platforms[3].height = 20;
    game->platforms[3].width = 60;
    game->platforms[3].x = 3000;
    game->platforms[3].y = 950;

    game->platforms[4].height = 20;
    game->platforms[4].width = 60;
    game->platforms[4].x = 3000;
    game->platforms[4].y = 650;

    game->platforms[5].height = 20;
    game->platforms[5].width = 60;
    game->platforms[5].x = 3000;
    game->platforms[5].y = 350;

    game->platforms[6].height = 20;
    game->platforms[6].width = 60;
    game->platforms[6].x = 3500;
    game->platforms[6].y = 800;

    game->platforms[7].height = 20;
    game->platforms[7].width = 60;
    game->platforms[7].x = 3500;
    game->platforms[7].y = 500;

    game->platforms[8].height = 20;
    game->platforms[8].width = 60;
    game->platforms[8].x = 3500;
    game->platforms[8].y = 200;

    game->platforms[9].height = 20;
    game->platforms[9].width = 600;
    game->platforms[9].x = 3950;
    game->platforms[9].y = 100;

    game->platforms[10].height = 20;
    game->platforms[10].width = 45;
    game->platforms[10].x = 4650;
    game->platforms[10].y = 1000;
    
    game->platforms[11].height = 100;
    game->platforms[11].width = 700;
    game->platforms[11].x = 5150;
    game->platforms[11].y = 1000;
    /*
    game->platforms[12].height = 20;
    game->platforms[12].width = 60;
    game->platforms[12].x = 3500;
    game->platforms[12].y = 200;
    
    game->platforms[13].height = 20;
    game->platforms[13].width = 60;
    game->platforms[13].x = 3500;
    game->platforms[13].y = 200;
    
    game->platforms[14].height = 20;
    game->platforms[14].width = 60;
    game->platforms[14].x = 3500;
    game->platforms[14].y = 200;
    
    game->platforms[15].height = 20;
    game->platforms[15].width = 60;
    game->platforms[15].x = 3500;
    game->platforms[15].y = 200;
    */

    game->healthItems[0].visible = true;
    game->healthItems[0].increasePoints = false;
    game->healthItems[0].increaseLives = true;
    game->healthItems[0].x = 3000.0f;
    game->healthItems[0].y = 450.0f;

    game->healthItems[1].visible = true;
    game->healthItems[1].increasePoints = false;
    game->healthItems[1].increaseLives = true;
    game->healthItems[1].x = 3500;
    game->healthItems[1].y = 750;

    game->pointItems[0].visible = true;
    game->pointItems[0].increasePoints = true;
    game->pointItems[0].increaseLives = true;
    game->pointItems[0].x = 3500;
    game->pointItems[0].y = 150;

    game->pointItems[1].visible = true;
    game->pointItems[1].increasePoints = true;
    game->pointItems[1].increaseLives = true;
    game->pointItems[1].x = 3550;
    game->pointItems[1].y = 50;
}
