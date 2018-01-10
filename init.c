#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_opengl.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_main.h"
#include "C:\Users\Tim Ruschke\Desktop\University\Prozedurale Programmierung\Project\misc\include\SDL2\SDL_ttf.h"
#include <stdbool.h>
#include <stdio.h>
#include "main.h"
#include "init.h"


void initHud(GameState *game){
    char livesStr[128] = "";
    sprintf(livesStr, "Lives: %d", game->hero.lives);
    // char timeStr[128] = "";
    // sprintf(timeStr, "%d", game->time);
    game->currentStage = "Draftlevel 24601";

    SDL_Color white = {255,255,255,255};

    SDL_Surface *tmpLives = TTF_RenderText_Blended(game->font, livesStr, white);
    SDL_Surface *tmpStageName = TTF_RenderText_Blended(game->font, game->currentStage, white);
    game->livesLabel = SDL_CreateTextureFromSurface(game->renderer, tmpLives);
    game->stageLabel = SDL_CreateTextureFromSurface(game->renderer, tmpStageName);
    SDL_FreeSurface(tmpLives);
    SDL_FreeSurface(tmpStageName);


}

void drawHud(GameState *game){
    SDL_Color white = {255,255,255,255};

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
    SDL_Rect livesRect = {20, 20, w, h};
    SDL_RenderCopy(game->renderer, game->livesLabel, NULL, &livesRect);
    // timeLabel
    SDL_QueryTexture(game->timeLabel, NULL, NULL, &w, &h);
    SDL_Rect timeRect = {640, 20, w, h};
    SDL_RenderCopy(game->renderer, game->timeLabel, NULL, &timeRect);
    // stageName
    SDL_QueryTexture(game->stageLabel, NULL, NULL, &w, &h);
    SDL_Rect stageRect = {300, 20, w, h};
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