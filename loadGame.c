#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include <SDL_image.h>
#include "hud.h"
#include "loadGame.h"
#include "main.h"
#include "collectible.h"
#include <string.h>

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
                                                SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    //Initialize renderer color
    SDL_SetRenderDrawColor( game->renderer, 0xFF, 0xFF, 0xFF, 0xFF );

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, 0);

    int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
    {
        printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
    }

    // Reset game state
    resetGame(game);

    // loading of texture
    game->hero.texture = loadTexture(game, "Assets/Textures/hero.png");
    game->sky = loadTexture(game, "Assets/Textures/sky.png");
    game->sea = loadTexture(game, "Assets/Textures/sea.png");
    game->clouds = loadTexture(game, "Assets/Textures/clouds.png");
    game->textureSet = loadTexture(game, "Assets/Textures/tileset.png");

    // spawnpoint
    game->spawnPoint[0].x = 300;
    game->spawnPoint[0].y = 30;
    

    // loading fonts (currently OpenFont does not work for some reason)
    
    game->font = TTF_OpenFont("Assets/Fonts/comicsans.ttf", 10);
    
    if(!game->font){
        exitGame(1, "Cannot find font file");
    }

    // AFTER loading font
    char msg[10] = "YOU WON!";
    SDL_Color color = {255, 255, 255, 255};
    game->winState = createTextureFromString(game, msg, color);

    char msgHint[20] = "PRESS R TO RESPAWN";
    SDL_Color colorHint = {111, 0, 25, 255};
    game->respawnHint = createTextureFromString(game, msgHint, colorHint);



    game->livesLabel = NULL;
    game->pointsLabel = NULL;
    initHud(game);
}

SDL_Texture* loadTexture(GameState *game, char *path)
{
    //The final texture
    SDL_Texture* newTexture = NULL;

    //Load image at specified path
    SDL_Surface* loadedSurface = IMG_Load(path);
    if(loadedSurface == NULL)
    {
        char msg[64];
        sprintf(msg, "Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        exitGame(1, msg);
    }
    else
    {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(game->renderer, loadedSurface);
        if(newTexture == NULL)
        {
            char msg[64];
            sprintf(msg, "Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
            exitGame(1, msg);
        }

        //Get rid of old loaded surface
        SDL_FreeSurface( loadedSurface );
    }

    return newTexture;
}

void exitGame(int errorCode, char* errorMsg){
    printf("\n%s\n", errorMsg);
    SDL_Quit();
    exit(1);
}

void resetGame(GameState *game){
    game->scrollX = 0;
    game->hero.x = 400;
    game->hero.y = 300;
    game->hero.width = 70.0f;//35.0f;
    game->hero.height = 178.0f;//89.0f;
    game->hero.dx = 0;
    game->hero.dy = 0;
    game->hero.maxdy = -10.0; //maxdy maybe as level of difficulty
    game->hero.jumping = false;
    game->hero.groundCollision = false;
    game->hero.lives = 3;
    game->hero.name = "Hero";
    game->hero.flip = SDL_FLIP_NONE;
    game->points = 0;
    game->isInWinState = false;
    game->spawnPoint[0].x = 400;
    game->spawnPoint[0].y = 300;
}

void loadPlatforms(GameState *game){
    // platforms
    for(int i = 0; i < N_PLATFORMS; i++){
        game->platforms[i].moveRight = false;
        game->platforms[i].deadly = false;
        game->platforms[i].collisionFree = false;
        SDL_Rect rect = {0,0,0,0};
        game->platforms[i].textureBox = rect;
        game->platforms[i].additionTop = 0.0f;
        game->platforms[i].width = 0;
        game->platforms[i].height = 0;
    }

    game->platforms[0].height = 400;
    game->platforms[0].width = 400;
    game->platforms[0].x = 400;
    game->platforms[0].y = 880;
    SDL_Rect tB = {192,186,48,38};
    game->platforms[0].textureBox = tB;
    game->platforms[0].additionTop = 30.0f;

    game->platforms[1].height = 400;
    game->platforms[1].width = 600;
    game->platforms[1].x = 1200;
    game->platforms[1].y = 750;
    game->platforms[1].textureBox = tB;

    game->platforms[2].height = 100;
    game->platforms[2].width = 400;
    game->platforms[2].x = 2400;
    game->platforms[2].y = 900;
    
    game->platforms[3].height = 1 ;
    game->platforms[3].width = 1;
    game->platforms[3].x = - 400;
    game->platforms[3].y = -400;
    
    game->platforms[4].height = 2*20;
    game->platforms[4].width = 2*60;
    game->platforms[4].x = 3000;
    game->platforms[4].y = 800;

    game->platforms[5].height = 2*20;
    game->platforms[5].width = 2*60;
    game->platforms[5].x = 3000;
    game->platforms[5].y = 400;
    
    game->platforms[6].height = 20;
    game->platforms[6].width = 60;
    game->platforms[6].x = -200;
    game->platforms[6].y = -300;
    
    game->platforms[7].height = 2*20;
    game->platforms[7].width = 2*60;
    game->platforms[7].x = 3500;
    game->platforms[7].y = 600;

    game->platforms[8].height = 2*20;
    game->platforms[8].width = 2*60;
    game->platforms[8].x = 3500;
    game->platforms[8].y = 200;

    game->platforms[9].height = 2*20;
    game->platforms[9].width = 700;
    game->platforms[9].x = 3950;
    game->platforms[9].y = 100;
    //moving
    
    game->platforms[10].height =2*20;
    game->platforms[10].width = 45;
    game->platforms[10].x = -4650;
    game->platforms[10].y = -1000;

    game->platforms[11].height = 100;
    game->platforms[11].width = 700;
    game->platforms[11].x = 5150;
    game->platforms[11].y = 1000;
    
    game->platforms[12].height = 600;
    game->platforms[12].width = 45;
    game->platforms[12].x = 5900;
    game->platforms[12].y = 0;
    game->platforms[12].moveRight = true;
    //moving
    game->platforms[13].height = 100;
    game->platforms[13].width = 45;
    game->platforms[13].x = 5900;
    game->platforms[13].y = 1080;
    game->platforms[13].moveRight = false;
    //moving
    
    game->platforms[14].height =2*20;
    game->platforms[14].width = 45;
    game->platforms[14].x = -6200;
    game->platforms[14].y = -850;
    
    game->platforms[15].height =2*20;
    game->platforms[15].width = 45;
    game->platforms[15].x = -6200;
    game->platforms[15].y = -700;

    game->platforms[16].height =2*20;
    game->platforms[16].width = 105;
    game->platforms[16].x = 6200;
    game->platforms[16].y = 960;
    
    game->platforms[17].height =2*20;
    game->platforms[17].width = 105;
    game->platforms[17].x = 6350;
    game->platforms[17].y = 785;

    game->platforms[18].height =2*20;
    game->platforms[18].width = 105;
    game->platforms[18].x = 6500;
    game->platforms[18].y = 610;

    game->platforms[19].height =2*20;
    game->platforms[19].width = 105;
    game->platforms[19].x = 6650;
    game->platforms[19].y = 435;
    
    game->platforms[20].height =2*20;
    game->platforms[20].width = 1250;
    game->platforms[20].x = 6750;
    game->platforms[20].y = 260;
    game->platforms[20].moveRight = true;
    //moving
    game->platforms[21].height = 30;
    game->platforms[21].width = 30;
    game->platforms[21].x = 6900;
    game->platforms[21].y = 150;
    game->platforms[21].deadly = true;
    //deadly
    game->platforms[22].height = 30;
    game->platforms[22].width = 30;
    game->platforms[22].x = 7100;
    game->platforms[22].y = 150;
    game->platforms[22].deadly = true;
    //deadly
    game->platforms[23].height = 30;
    game->platforms[23].width = 30;
    game->platforms[23].x = 7300;
    game->platforms[23].y = 150;
    game->platforms[23].deadly = true;
    //deadly
    game->platforms[24].height = 30;
    game->platforms[24].width = 30;
    game->platforms[24].x = 7500;
    game->platforms[24].y = 150;
    game->platforms[24].deadly = true;
    //deadly
    game->platforms[25].height = 30;
    game->platforms[25].width = 30;
    game->platforms[25].x = 7650;
    game->platforms[25].y = 150;
    game->platforms[25].deadly = true;
    //deadly
    game->platforms[26].height = 980;
    game->platforms[26].width = 500;
    game->platforms[26].x = 8000;
    game->platforms[26].y = 100;

    game->platforms[27].height = 860;
    game->platforms[27].width = 200;
    game->platforms[27].x = 8500;
    game->platforms[27].y = 220;

    game->platforms[28].height = 740;
    game->platforms[28].width = 200;
    game->platforms[28].x = 8650;
    game->platforms[28].y = 340;
    //
    game->platforms[29].height =2*20;
    game->platforms[29].width = 200;
    game->platforms[29].x = 9000;
    game->platforms[29].y = 475;

    game->platforms[30].height =2*20;
    game->platforms[30].width = 200;
    game->platforms[30].x = 9000;
    game->platforms[30].y = 245;
    game->platforms[30].collisionFree = true;

    game->platforms[31].height = 2*20;
    game->platforms[31].width = 200;
    game->platforms[31].x = 9000;
    game->platforms[31].y = -40;
    //
    game->platforms[32].height =2*20;
    game->platforms[32].width = 200;
    game->platforms[32].x = 9400;
    game->platforms[32].y = 475;

    game->platforms[33].height =2*20;
    game->platforms[33].width = 200;
    game->platforms[33].x = 9400;
    game->platforms[33].y = 245;
    game->platforms[33].collisionFree = true;

    game->platforms[34].height =2*20;
    game->platforms[34].width = 200;
    game->platforms[34].x = 9400;
    game->platforms[34].y = -40;
    //
    game->platforms[35].height =2*20;
    game->platforms[35].width = 200;
    game->platforms[35].x = 9800;
    game->platforms[35].y = 475;

    game->platforms[36].height =2*20;
    game->platforms[36].width = 200;
    game->platforms[36].x = 9800;
    game->platforms[36].y = 245;
    game->platforms[36].collisionFree = true;

    game->platforms[37].height =2*20;
    game->platforms[37].width = 200;
    game->platforms[37].x = 9800;
    game->platforms[37].y = -40;
    //
    game->platforms[38].height = 1005;
    game->platforms[38].width = 400;
    game->platforms[38].x = 10400;
    game->platforms[38].y = 75;

    game->platforms[39].height =2*20;
    game->platforms[39].width = 640;
    game->platforms[39].x = 10800;
    game->platforms[39].y = 75;

    game->platforms[40].height = 510;
    game->platforms[40].width =2*20;
    game->platforms[40].x = 11400;
    game->platforms[40].y = 115;
    game->platforms[40].deadly = true;

    game->platforms[41].height =2*20;
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
    game->pointItems[0].increaseLives = false;
    game->pointItems[0].x = 3000.0f;
    game->pointItems[0].y = 450.0f;

    game->pointItems[1].visible = true;
    game->pointItems[1].increasePoints = true;
    game->pointItems[1].increaseLives = false;
    game->pointItems[1].x = 3020.0f;
    game->pointItems[1].y = 450.0f;
}