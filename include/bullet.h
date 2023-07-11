#ifndef __BULLET_H__
#define __BULLET_H__
#include "raylib.h"

typedef struct bullet {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    Color color;
} bullet;

bullet Bullet(Vector2 pos, Vector2 size, Vector2 vel, Color color);

#endif
