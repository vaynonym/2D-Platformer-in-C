#include <stdio.h>
#include <stdbool.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include "main.h"
#include "collisionDetection.h"
#include "collectible.h"
#include "loadGame.h"

#define ABS(x) (x < 0 ? -x : x)

// current Problems:
/*
- the hero can jump after hitting a platform from below, continiously pressing him against the platform until he leaves it from the x-coordinate resulting in a normal jump
  presumably this is because groundCollission is set on true independently of whether the hero hits a platform from below or above
  this could be checked by looking at whether dy of the hero is positive or negative (?)
- the hero's y coordinate is not adjusted when pressing against a platform from the side. This only applies if the player continuously pressed against the platform.
  this presumably means the y-coordinate is not adjusted if the player's x movement would cause him to be inside the platform/hit the platform
  however, we want the y coordinate to always be adjusted by dy if it would not cause the player to be inside a platform. 
  all the while dy is continuously increasing, just not being added to the y position of the hero.


*/
/*

// old attempt

bool isColliding(GameState *game, double vectorX, double vectorY, bool debug){
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
            
            if(i == 9 || i == 20){
                game->hero.onMoving = true;
            }
            
            if(debug){ 
                printf("x: %d y: %d w: %d h: %d\n",intersection.x, intersection.y, intersection.w, intersection.h);
            }

        }
        else if(intersection.h > 0 && intersection.w > 0){
            return true;
        }
    }
    return false;
}

void detectCollision(GameState *game){


    // tempX and tempY are the coordinates the hero would end up at with the current movement
    // hero.x and hero.y are the coordinates of the hero from the last frame

    //Vector approximation
    double vectorX, vectorY, scaledVectorX, scaledVectorY;
    vectorX = game->hero.tempX - game->hero.x; // the movement vector for the x coordinate
    vectorY = game->hero.tempY - game->hero.y; // the movement vector for the y coordinate
    double scalingFactor, scalingStart;

    game->hero.groundCollision = false;

    //Validating movement
    if(!isColliding(game, vectorX, vectorY, false)){
        game->hero.x = game->hero.tempX;
        if(!game->hero.groundCollision) 
            game->hero.y = game->hero.tempY;
        return;
    }
    game->hero.groundCollision = false;

    //Otherwise trying to scale vector to keep heading unless vector is zero vector
    scalingStart = scalingFactor = (vectorX > vectorY ? vectorX : vectorY);
    //Vector with scalingFactor 1 got already tested as trivial case
    while(scalingFactor > 1){
        scalingFactor--;
        scaledVectorX = (vectorX / scalingStart) * scalingFactor;
        scaledVectorY = (vectorY / scalingStart) * scalingFactor;

        if(! isColliding(game, scaledVectorX, scaledVectorY, false)){ // if the hero does not collide with any object with the new vector, scaling completed
            //Scaled vector points to no collision
            game->hero.x = game->hero.x + scaledVectorX; 
            if(!game->hero.groundCollision) // if the hero does not collide with the ground, he will keep moving in Y direction. 
                game->hero.y = game->hero.y + scaledVectorY; // Question: When does ground collission apply? It should only apply when the hero hits a platform from above or below, then.
            return; // since the hero gets stuck in the y direction when he hits a platform from the side, I presume 
        }
    }

    game->hero.groundCollision = false;
    //If reached vector scaling does not change the detected collision.
    if(isColliding(game, 0.0f, 0.0f,true)){
        //Respawn
        respawn(game);
    }
}

*/





void detectCollision(GameState *game){
    bool collided;
    game->hero.groundCollision = false; // whether the hero is on the ground has to be determined each frame
    for(int i = 0; i < N_PLATFORMS; i++){ // check every platform
        collided = false;
        if(game->platforms[i].x + game->scrollX - game->platforms[i].width <= width){ // that is visible on the screen
            // shortcuts
            StaticObject platform = game->platforms[i];
            Character hero = game->hero;

            if(platform.collisionFree) // ignore platforms that should not be collided with
                continue;

            // hero Edges
            double hEdgeRight = hero.x + hero.width;
            double hEdgeLeft  = hero.x;
            double hEdgeBot   = hero.y + hero.height;
            double hEdgeTop   = hero.y;

            // platform Edges
            double pEdgeRight = platform.x + platform.width;
            double pEdgeLeft  = platform.x;
            double pEdgeBot   = platform.y + platform.height;
            double pEdgeTop   = platform.y;    

            if(hEdgeBot > pEdgeTop && hEdgeTop < pEdgeBot && game->hero.dy > 0.0f){ // y position within the platform
                if(hEdgeLeft < pEdgeRight && hEdgeRight > pEdgeRight && hero.dx < 0.0f){ // hitting from right side
                    game->hero.x = pEdgeRight; // put hero outside of box
                    hEdgeRight = game->hero.x + hero.width; // update the Edges
                    hEdgeLeft = game->hero.x;
                    game->hero.dx = 0.0f;
                    collided = true;
                }
                else if(hEdgeRight > pEdgeLeft && hEdgeLeft < pEdgeLeft && hero.dx >0.0f){  // hitting from left side
                    game->hero.x = pEdgeLeft - hero.width; // put hero outside of box
                    hEdgeRight = game->hero.x + hero.width; // update the Edges
                    hEdgeLeft = game->hero.x;
                    game->hero.dx =0.0f;
                    collided = true;
                    }
                }
            if(hero.x + hero.width / 2 > pEdgeLeft && hero.x+hero.width/2 < pEdgeRight)
            {
            //are we bumping our head?
                if(hero.y < pEdgeBot && hero.y > pEdgeTop && hero.dy < 0)
                {
                    game->hero.y = pEdgeBot;
                    hEdgeRight = game->hero.x + hero.width; // update the Edges
                    hEdgeLeft = game->hero.x;
                    
                    game->hero.dy = 0;
                }
            }


            if(hEdgeRight > pEdgeLeft && hEdgeLeft < pEdgeRight){ // within x-range
                /*if(hEdgeTop < pEdgeBot && hEdgeTop > pEdgeTop && game->hero.dy < 0.0f){        // hitting from below
                    game->hero.y = pEdgeBot;
                    hEdgeBot   = game->hero.y + hero.height;
                    hEdgeTop   = game->hero.y;
                    game->hero.dy =0.0f; // lose momentum
                    collided = true;
                }
                else */if(hEdgeBot > pEdgeTop && hEdgeTop < pEdgeTop && game->hero.dy >0.0f){       // hitting from above
                    game->hero.y = pEdgeTop - hero.height;
                    hEdgeBot   = game->hero.y + hero.height;
                    hEdgeTop   = game->hero.y;
                    game->hero.dy =0.0f;
                    collided = true;
                    // set groundCollision true, reset maxdy to allow for jumping
                    game->hero.groundCollision = true;
                    game->hero.maxdy = -10;
                }
            }
            if(collided && platform.deadly){
                respawn(game);
                printf("respawn");
                // respawning
            }
        }
    }
}

void setSpawnpoint (GameState *game, Collectible collectible){
    game->spawnPoint[0].x = collectible.x + game->hero.width / 2;
    game->spawnPoint[0].y = collectible.y + game->hero.height;
}

void respawn(GameState *game){
    game->hero.dx =0.0f;
    game->hero.dy =0.0f;
    game->hero.lives--;
    
    if(game->hero.lives == 0 ){ // gameover. Restarting game.
        resetGame(game);
    }
    else{ // normal respawn
        game->hero.x = game->spawnPoint[0].x;
        game->hero.y = game->spawnPoint[0].y;
    }
    game->updateHud = true; // update the now changed lives
}
