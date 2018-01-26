#include <stdbool.h>
#include <stdio.h>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SDL_main.h"
#include "SDL_ttf.h"
#include "main.h"
#include "init.h"


void initHud(GameState *game){

    game->currentStage = "Draftlevel 24601";
    char livesStr[12] = "";
    sprintf(livesStr, "Lives: %d", game->hero.lives);

    SDL_Color white = {255,255,255,255};

    SDL_Surface *tmpLives = TTF_RenderText_Blended(game->font, livesStr, white);
    SDL_Surface *tmpStageName = TTF_RenderText_Blended(game->font, game->currentStage, white);

    game->stageLabel = SDL_CreateTextureFromSurface(game->renderer, tmpStageName);
    game->livesLabel = SDL_CreateTextureFromSurface(game->renderer, tmpLives);
    
    SDL_FreeSurface(tmpStageName);
    SDL_FreeSurface(tmpLives);


}

void drawHud(GameState *game){

    SDL_Color white = {255,255,255,255};

    if(game->updateHud){ // dynamically update lives after a respawn
        char livesStr[12] = "";
        sprintf(livesStr, "Lives: %d", game->hero.lives);
        SDL_Surface *tmpLives = TTF_RenderText_Blended(game->font, livesStr, white);
        game->livesLabel = SDL_CreateTextureFromSurface(game->renderer, tmpLives);
        SDL_FreeSurface(tmpLives);
        game->updateHud = false;
    }

    // Makes a Texture for Time, dynamically updated every 60 frames. Corresponds roughly with time in seconds if run at 60 fps
    if (game->time%60 == 0){
        char timeStr[128] = "";
        sprintf(timeStr, "%d", game->time / 60);
        SDL_Surface *tmpTime = TTF_RenderText_Blended(game->font, timeStr, white);
        game->timeLabel = SDL_CreateTextureFromSurface(game->renderer, tmpTime);
        SDL_FreeSurface(tmpTime);
    }
    
    // draw in white
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    int w, h; //texture width and height
    // livesLabel
    SDL_QueryTexture(game->livesLabel, NULL, NULL, &w, &h);
    SDL_Rect livesRect = {200, 30, w, h};
    SDL_RenderCopy(game->renderer, game->livesLabel, NULL, &livesRect);
    // timeLabel
    SDL_QueryTexture(game->timeLabel, NULL, NULL, &w, &h);
    SDL_Rect timeRect = {1600, 30, w, h};
    SDL_RenderCopy(game->renderer, game->timeLabel, NULL, &timeRect);
    // stageName
    SDL_QueryTexture(game->stageLabel, NULL, NULL, &w, &h);
    SDL_Rect stageRect = {800, 30, w, h};
    SDL_RenderCopy(game->renderer, game->stageLabel, NULL, &stageRect);

}

void clearHud(GameState *game){
    if(game->livesLabel != NULL){
        SDL_DestroyTexture(game->livesLabel);
    }
    if(game->timeLabel != NULL){
        SDL_DestroyTexture(game->timeLabel);
    }
}