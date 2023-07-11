#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <stdint.h>
#include "raylib.h"
#include "bullet.h"

typedef struct player {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    Color color;
    uint32_t score;
    uint32_t max_bullets;
    uint32_t amount_of_bullets;
    bullet bullets[5];
} player;

player Player(Vector2 pos, Vector2 size, Vector2 vel, Color color);
void shoot(player *p);
void update_player(player *p, double *bullet_time, float delta_time);
void draw_player(player p);
void update_bullets(player *p, float delta_time);
void draw_bullets(player p);

#endif
