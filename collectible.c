#include <stdbool.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include "main.h"
#include "collisionDetection.h"
#include "collectible.h"



#include <stdio.h>

void testForAllCollectibles(GameState *game){
    SDL_Rect rectHero = {game->hero.x, game->hero.y, game->hero.width, game->hero.height};
    
    testForCollectibles(rectHero, game, N_HEALTH, game->healthItems);
    testForCollectibles(rectHero, game, N_POINTS, game->pointItems);
}

void testForCollectibles(SDL_Rect rectHero, GameState *game, int max, Collectible *collectibles){
    for(int i = 0; i < max; i++){
        Collectible collectible = collectibles[i];

        if(collectible.visible){
            SDL_Rect rectCollectible = {collectible.x, collectible.y, 50.0f, 50.0f};

            if(SDL_HasIntersection(&rectHero, &rectCollectible)){
                collectibles[i].visible = false;
                game->updateHud = true;

                if(collectible.increasePoints){
                    game->points = game->points + 1;

                    if(game->points == N_POINTS){
                        game->isInWinState = true;
                    }
                    setSpawnpoint(game, collectibles[i]);
                }
                if(collectible.increaseLives){
                    game->hero.lives = game->hero.lives + 1;
                }
            }
        }
    }
}

void resetCollectibles(GameState *game){
    game->points = 0;

    for(int i = 0; i < N_POINTS; i++){
        game->pointItems[i].visible = true;
    }
    for(int i = 0; i < N_HEALTH; i++){
        game->healthItems[i].visible = true;
    }
}