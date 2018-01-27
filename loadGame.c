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
    game->spawnPoint[0].y = 30;
    

    // loading fonts (currently OpenFont does not work for some reason)
    
    game->font = TTF_OpenFont("Assets/Fonts/comicsans.ttf", 24);
    
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
    game->platforms[9].width = 120;
    game->platforms[9].x = 3950;
    game->platforms[9].y = 100;
    //moving
    game->platforms[10].height = 20;
    game->platforms[10].width = 45;
    game->platforms[10].x = 4650;
    game->platforms[10].y = 1000;
    
    game->platforms[11].height = 100;
    game->platforms[11].width = 700;
    game->platforms[11].x = 5150;
    game->platforms[11].y = 1000;
    
    game->platforms[12].height = 500;
    game->platforms[12].width = 45;
    game->platforms[12].x = 5900;
    game->platforms[12].y = 100;
    game->platforms[12].moveRight = true;
    //moving
    game->platforms[13].height = 100;
    game->platforms[13].width = 45;
    game->platforms[13].x = 5900;
    game->platforms[13].y = 950;
    game->platforms[13].moveRight = false;
    //moving
    game->platforms[14].height = 20;
    game->platforms[14].width = 45;
    game->platforms[14].x = 6200;
    game->platforms[14].y = 850;
    
    game->platforms[15].height = 20;
    game->platforms[15].width = 45;
    game->platforms[15].x = 6200;
    game->platforms[15].y = 700;

    game->platforms[16].height = 20;
    game->platforms[16].width = 45;
    game->platforms[16].x = 6200;
    game->platforms[16].y = 550;

    game->platforms[17].height = 20;
    game->platforms[17].width = 45;
    game->platforms[17].x = 6200;
    game->platforms[17].y = 400;

    game->platforms[18].height = 20;
    game->platforms[18].width = 45;
    game->platforms[18].x = 6200;
    game->platforms[18].y = 250;

    game->platforms[19].height = 20;
    game->platforms[19].width = 45;
    game->platforms[19].x = 6200;
    game->platforms[19].y = 100;
    
    game->platforms[20].height = 20;
    game->platforms[20].width = 120;
    game->platforms[20].x = 6400;
    game->platforms[20].y = 100;
    game->platforms[20].moveRight = true;
    //moving
    game->platforms[21].height = 30;
    game->platforms[21].width = 30;
    game->platforms[21].x = 6700;
    game->platforms[21].y = 70;
    game->platforms[21].deadly = true;
    //deadly
    game->platforms[22].height = 30;
    game->platforms[22].width = 30;
    game->platforms[22].x = 6900;
    game->platforms[22].y = 70;
    game->platforms[22].deadly = true;
    //deadly
    game->platforms[23].height = 30;
    game->platforms[23].width = 30;
    game->platforms[23].x = 7100;
    game->platforms[23].y = 70;
    game->platforms[23].deadly = true;
    //deadly
    game->platforms[24].height = 30;
    game->platforms[24].width = 30;
    game->platforms[24].x = 7300;
    game->platforms[24].y = 70;
    game->platforms[24].deadly = true;
    //deadly
    game->platforms[25].height = 30;
    game->platforms[25].width = 30;
    game->platforms[25].x = 7500;
    game->platforms[25].y = 70;
    game->platforms[25].deadly = true;

    game->platforms[26].height = 980;
    game->platforms[26].width = 400;
    game->platforms[26].x = 8000;
    game->platforms[26].y = 100;

    game->platforms[27].height = 860;
    game->platforms[27].width = 200;
    game->platforms[27].x = 8400;
    game->platforms[27].y = 220;

    game->platforms[28].height = 740;
    game->platforms[28].width = 200;
    game->platforms[28].x = 8600;
    game->platforms[28].y = 340;
    //
    game->platforms[29].height = 20;
    game->platforms[29].width = 200;
    game->platforms[29].x = 9000;
    game->platforms[29].y = 475;

    game->platforms[30].height = 20;
    game->platforms[30].width = 200;
    game->platforms[30].x = 9000;
    game->platforms[30].y = 245;

    game->platforms[31].height = 20;
    game->platforms[31].width = 200;
    game->platforms[31].x = 9000;
    game->platforms[31].y = -20;
    //
    game->platforms[32].height = 20;
    game->platforms[32].width = 200;
    game->platforms[32].x = 9400;
    game->platforms[32].y = 475;

    game->platforms[33].height = 20;
    game->platforms[33].width = 200;
    game->platforms[33].x = 9400;
    game->platforms[33].y = 245;

    game->platforms[34].height = 20;
    game->platforms[34].width = 200;
    game->platforms[34].x = 9400;
    game->platforms[34].y = -20;
    //
    game->platforms[35].height = 20;
    game->platforms[35].width = 200;
    game->platforms[35].x = 9800;
    game->platforms[35].y = 475;

    game->platforms[36].height = 20;
    game->platforms[36].width = 200;
    game->platforms[36].x = 9800;
    game->platforms[36].y = 245;

    game->platforms[37].height = 20;
    game->platforms[37].width = 200;
    game->platforms[37].x = 9800;
    game->platforms[37].y = -20;
    //
    game->platforms[38].height = 1005;
    game->platforms[38].width = 400;
    game->platforms[38].x = 10400;
    game->platforms[38].y = 75;

    game->platforms[39].height = 20;
    game->platforms[39].width = 600;
    game->platforms[39].x = 10800;
    game->platforms[39].y = 75;

    game->platforms[40].height = 810;
    game->platforms[40].width = 20;
    game->platforms[40].x = 11400;
    game->platforms[40].y = 75;
    game->platforms[40].deadly = true;

    game->platforms[41].height = 20;
    game->platforms[41].width = 600;
    game->platforms[41].x = 10800;
    game->platforms[41].y = 1060;

    /*
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
