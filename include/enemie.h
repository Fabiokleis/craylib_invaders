#ifndef __ENEMIE_H__
#define __ENEMIE_H__
#include <raylib.h>
#include <stdint.h>
#include "config.h"
#include "bullet.h"
#include "player.h"

typedef struct {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    Color color;
    uint32_t shoot_time;
} enemie;


enemie Enemie(Vector2 pos, Vector2 size, Vector2 vel, Color color, uint32_t rand);
void update_enemies(
    player *p,
    enemie enemies[ENEMIE_ROWS][ENEMIE_COLS],
    bullet enemie_bullets[ENEMIE_BULLETS],
    uint32_t *bullet_counter,
    double *enemie_shoot_time,
    double delta_time,
    int direction_step                    
);
bool are_all_dead(enemie enemies[ENEMIE_ROWS][ENEMIE_COLS]);
void draw_enemies(enemie enemies[ENEMIE_ROWS][ENEMIE_COLS], bullet enemie_bullets[ENEMIE_BULLETS], uint32_t bullet_counter);

#endif
