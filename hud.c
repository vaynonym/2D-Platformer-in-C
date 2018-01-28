#include <stdbool.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include "main.h"
#include "hud.h"

SDL_Color white = {255,255,255,255};

void initHud(GameState *game){
    game->currentStage = "Draftlevel 24601";
    char livesStr[12] = "";
    sprintf(livesStr, "Lives: %d", game->hero.lives);

    char pointsStr[12] = "";
    sprintf(pointsStr, "Points: %d", game->points);


    game->stageLabel = createTextureFromString(game, game->currentStage);
    game->livesLabel = createTextureFromString(game, livesStr);
    game->pointsLabel = createTextureFromString(game, pointsStr);
}

void drawHud(GameState *game){

    SDL_Color white = {255,255,255,255};

    if(game->updateHud){ // dynamically update lives after a respawn
        char livesStr[12] = "";
        sprintf(livesStr, "Lives: %d", game->hero.lives);
        game->livesLabel = createTextureFromString(game, livesStr);

        char pointsStr[12] = "";
        sprintf(pointsStr, "Points: %d", game->points);
        game->pointsLabel = createTextureFromString(game, pointsStr);

        game->updateHud = false;
    }

    // draw in white
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    int w, h; //texture width and height
    // livesLabel
    SDL_QueryTexture(game->livesLabel, NULL, NULL, &w, &h);

    int scale = 4;

    SDL_Rect rect = {0, 1040-h*scale, 1920, 40+h*scale};
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(game->renderer, &rect);

    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);

    SDL_Rect livesRect = {200, 1060-h*scale, w*scale, h*scale};
    SDL_RenderCopy(game->renderer, game->livesLabel, NULL, &livesRect);

    SDL_QueryTexture(game->pointsLabel, NULL, NULL, &w, &h);
    SDL_Rect pointsRect = {1600, 1060-h*scale, w*scale, h*scale};
    SDL_RenderCopy(game->renderer, game->pointsLabel, NULL, &pointsRect);

    SDL_QueryTexture(game->stageLabel, NULL, NULL, &w, &h);
    SDL_Rect stageRect = {800, 1060-h*scale, w*scale, h*scale};
    SDL_RenderCopy(game->renderer, game->stageLabel, NULL, &stageRect);
}

void clearHud(GameState *game){
    if(game->livesLabel != NULL){
        SDL_DestroyTexture(game->livesLabel);
    }
    if(game->pointsLabel != NULL){
        SDL_DestroyTexture(game->pointsLabel);
    }
}

SDL_Texture* createTextureFromString(GameState *game, char* text){
    SDL_Surface *surface = TTF_RenderText_Blended(game->font, text, white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(game->renderer, surface);
    SDL_FreeSurface(surface);
    return texture;
}