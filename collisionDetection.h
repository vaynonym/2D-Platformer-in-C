#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include "main.h"


// bool isColliding(GameState *game, double vectorX, double vectorY, bool debug);

void detectCollision(GameState *game);

void setSpawnpoint(GameState *game, Collectible collectible);

void respawn(GameState *game);

#endif