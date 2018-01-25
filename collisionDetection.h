#ifndef COLLISIONDETECTION_H
#define COLLISIONDETECTION_H
#include "main.h"


bool isColliding(GameState *game, float vectorX, float vectorY, bool debug);

void detectCollision(GameState *game);

#endif