#include <raylib.h>
#include <stdbool.h>
#include "config.h"
#include "enemie.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

enemie Enemie(Vector2 pos, Vector2 size, Vector2 vel, Color color, uint32_t rand) {
    return (enemie){
        .position = pos,
        .size = size,
        .velocity = vel,
        .color = color,
        .shoot_time = rand,
    };
}

void update_enemies(
    enemie enemies[ENEMIE_ROWS][ENEMIE_COLS],
    bullet enemie_bullets[ENEMIE_BULLETS],
    uint32_t *bullet_counter,
    double *enemie_shoot_time,
    double delta_time,
    int direction_step
) {

    srand(time(NULL));
    if (*enemie_shoot_time >= 4.0) {
        *enemie_shoot_time = 0.0;
       
        for (uint32_t i = 0; i < ENEMIE_ROWS; ++i) {
            for (uint32_t j = 0; j < ENEMIE_COLS; ++j) {
                enemies[i][j].shoot_time--;
                if (enemies[i][j].shoot_time == 0) {
                    
                    enemies[i][j].shoot_time = 1 + rand() % ENEMIE_SHOOT_TIME;
                    Vector2 b_size = {enemies[i][j].size.x/3, enemies[i][j].size.y};
                    Vector2 b_pos = {enemies[i][j].position.x + enemies[i][j].size.x/2 - b_size.x/2, enemies[i][j].position.y};
                    Vector2 b_vel = {0.0f, 500.0f};
                                        
                    enemie_bullets[(*bullet_counter)] = Bullet(b_pos, b_size, b_vel, DARKBLUE);
                    (*bullet_counter)++;
                }
            }


        }
    }

    for (uint32_t i = 0; i < ENEMIE_ROWS; ++i) {
        for (uint32_t j = 0; j < ENEMIE_COLS; ++j) {
            enemies[i][j].position.x += enemies[i][j].velocity.x * delta_time * direction_step;
            enemies[i][j].position.y += enemies[i][j].velocity.y * delta_time;
        }
    }

    for (uint32_t b = 0; b < *bullet_counter; ++b) {
        enemie_bullets[b].position.y += enemie_bullets[b].velocity.y * delta_time;
        if (enemie_bullets[b].position.y >= HEIGHT) {
            enemie_bullets[b] = enemie_bullets[--(*bullet_counter)];
        }
    }
}

void draw_enemies(enemie enemies[ENEMIE_ROWS][ENEMIE_COLS], bullet enemie_bullets[ENEMIE_BULLETS], uint32_t bullet_counter) {
    for (uint32_t i = 0; i < ENEMIE_ROWS; ++i) {
        for (uint32_t j = 0; j < ENEMIE_COLS; ++j) {
            DrawRectangleV(enemies[i][j].position, enemies[i][j].size, enemies[i][j].color); 
        }
    }

    for (uint32_t i = 0; i < bullet_counter; ++i) {
        DrawRectangleV(enemie_bullets[i].position, enemie_bullets[i].size, enemie_bullets[i].color);
    }
}