#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include "main.h"
#include "collisionDetection.h"

bool isColliding(GameState *game, float vectorX, float vectorY, bool debug){
    for(int i = 0; i < 42; i++){ //PLATFORMS
        StaticObject platform = game->platforms[i];

        SDL_Rect rectHero;
        SDL_Rect rectPlatform;
        SDL_Rect intersection;

        rectHero.x = game->hero.x + vectorX;
        rectHero.y = game->hero.y + vectorY;
        rectHero.w = game->hero.width;
        rectHero.h = game->hero.height;
        rectPlatform.x = platform.x;
        rectPlatform.y = platform.y;
        rectPlatform.w = platform.width;
        rectPlatform.h = platform.height;

        SDL_IntersectRect(&rectHero, &rectPlatform, &intersection);

        //Standing on ground is not a collision - +- 1 Pixel?
        if(intersection.h == 0 && intersection.w > 0){
            game->hero.dy = 0.0f;
            vectorY = 0.0f;
            game->hero.groundCollision = true;
            game->hero.maxdy = -10.0f;
            
            if(i == 9 || i == 20){
                game->hero.onMoving = true;
            }
            
            if(debug){ 
                printf("x: %d y: %d w: %d h: %d\n",intersection.x, intersection.y, intersection.w, intersection.h);
            }
        }
        else if(intersection.w > 0 && intersection.h > 0){
            return true;
        }
    }
    return false;
}

void detectCollision(GameState *game){
    
    /*
    SET VALUES HERE TO AVOID ROUNDING DIFFERENCES
    */

    //Vector approximation
    float vectorX, vectorY, scaledVectorX, scaledVectorY;
    vectorX = game->hero.tempX - game->hero.x;
    vectorY = game->hero.tempY - game->hero.y;
    int scalingFactor, scalingStart;

    game->hero.groundCollision = false;

    //Validating movement
    if(!isColliding(game, vectorX, vectorY,false)){
        game->hero.x = game->hero.tempX;
        if(!game->hero.groundCollision) 
            game->hero.y = game->hero.tempY;
        return;
    }
    game->hero.groundCollision = false;

    //Otherwise trying to scale vector to keep heading unless vector is zero vector
    scalingStart = scalingFactor = (int) (vectorX > vectorY ? vectorX : vectorY);
    //Vector with scalingFactor 1 got already tested as trivial case
    while(scalingFactor > 1){
        scalingFactor--;
        scaledVectorX = (vectorX / (float)scalingStart) * scalingFactor;
        scaledVectorY = (vectorY / (float)scalingStart) * scalingFactor;

        if(!isColliding(game, scaledVectorX, scaledVectorY,false)){
            //Scaled vector points to no collision
            game->hero.x = game->hero.x + scaledVectorX;
            if(!game->hero.groundCollision) 
                game->hero.y = game->hero.y + scaledVectorY;
            return;
        }
    }

    game->hero.groundCollision = false;
    //If reached vector scaling does not change the detected collision.
    if(isColliding(game, 0.0f, 0.0f,true)){
        //Respawn
        respawn(game);
    }
}