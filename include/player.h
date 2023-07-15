#ifndef __PLAYER_H__
#define __PLAYER_H__
#include <stdint.h>
#include <raylib.h>
#include <stdbool.h>
#include "bullet.h"


typedef struct {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    Color color;
    uint32_t score;
    uint32_t max_bullets;
    uint32_t amount_of_bullets;
    bullet bullets[5];
    uint32_t life;
    Sound bullet_sound;
} player;

player Player(Vector2 pos, Vector2 size, Vector2 vel, Color color, Sound bullet_sound);
void shoot(player *p);
float sat_collision_detect(Vector2 pos1, Vector2 size1, Vector2 pos2, Vector2 size2);
bool is_colliding_squares(Vector2 pos1, Vector2 size1, Vector2 pos2, Vector2 size2);
void update_player(player *p, double *bullet_time, float delta_time);
void draw_player(player p);
void update_bullets(player *p, float delta_time);
void draw_bullets(player p);

#endif
