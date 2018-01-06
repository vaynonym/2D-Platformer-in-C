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
    SDL_Color white = {255,255,255,255};

    SDL_Surface *tmpSur = TTF_RenderText_Blended(game->font, livesStr, white);
    game->label = SDL_CreateTextureFromSurface(game->renderer, tmpSur);
    SDL_FreeSurface(tmpSur);
}

void drawHud(GameState *game){
    // draw in white
    SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
    int w, h; //texture width and height
    SDL_QueryTexture(game->label, NULL, NULL, &w, &h);
    SDL_Rect textRect = {40 + game->scrollX , 40, w, h};

}

void clearHud(GameState *game){
    if(game->label != NULL){
        SDL_DestroyTexture(game->label);
    }
}