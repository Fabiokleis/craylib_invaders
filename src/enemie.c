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
    player *p,
    enemie enemies[ENEMIE_ROWS][ENEMIE_COLS],
    bullet enemie_bullets[ENEMIE_BULLETS],
    uint32_t *bullet_counter,
    double *enemie_shoot_time,
    double delta_time,
    int direction_step
) {

    srand(time(NULL));
    if (*enemie_shoot_time >= 1.0) {
        *enemie_shoot_time = 0.0;
       
        for (uint32_t i = 0; i < ENEMIE_ROWS; ++i) {
            for (uint32_t j = 0; j < ENEMIE_COLS; ++j) {
                if (enemies[i][j].position.x == -1 && enemies[i][j].position.y == -1)
                    continue;
                

                if (enemies[i][j].shoot_time == 0) {
                    
                    enemies[i][j].shoot_time = 1 + rand() % ENEMIE_SHOOT_TIME;
                    Vector2 b_size = {enemies[i][j].size.x/4, enemies[i][j].size.y/2};
                    Vector2 b_pos = {enemies[i][j].position.x + enemies[i][j].size.x/2 - b_size.x/2, enemies[i][j].position.y};
                    Vector2 b_vel = {0.0f, 700.0f};
                                        
                    enemie_bullets[(*bullet_counter)++] = Bullet(b_pos, b_size, b_vel, PURPLE);
                }
                enemies[i][j].shoot_time--;
            }
        }
    }

    for (uint32_t i = 0; i < ENEMIE_ROWS; ++i) {
        for (uint32_t j = 0; j < ENEMIE_COLS; ++j) {
            if (enemies[i][j].position.x == -1 && enemies[i][j].position.y == -1)
                    continue;

            if (enemies[i][j].position.y + enemies[i][j].size.y >= HEIGHT)
                p->life = 0;
            
            enemies[i][j].position.x += enemies[i][j].velocity.x * delta_time * direction_step;
            enemies[i][j].position.y += enemies[i][j].velocity.y * delta_time;
        }
    }
    
    for (uint32_t i = 0; i < p->amount_of_bullets; ++i) {
        for (uint32_t j = 0; j < ENEMIE_ROWS; ++j) {
            for (uint32_t k = 0; k < ENEMIE_COLS; ++k) {
                if (enemies[j][k].position.x == -1 && enemies[j][k].position.y == -1)
                    continue;
                            
                if (is_colliding_squares(p->bullets[i].position, p->bullets[i].size, enemies[j][k].position, enemies[j][k].size)) {                        
                    enemies[j][k].position.x = -1;
                    enemies[j][k].position.y = -1;
                    p->score++;
                    p->bullets[i] = p->bullets[--(p->amount_of_bullets)];
                }
            }
        }        
    }

    for (uint32_t b = 0; b < *bullet_counter; ++b) {
        enemie_bullets[b].position.y += enemie_bullets[b].velocity.y * delta_time;
        if (is_colliding_squares(p->position, p->size, enemie_bullets[b].position, enemie_bullets[b].size)) {
            enemie_bullets[b] = enemie_bullets[--(*bullet_counter)];
            p->life--;
            if (p->life == 0)
                printf("game over!\n");
        }
        if (enemie_bullets[b].position.y >= HEIGHT) {
            enemie_bullets[b] = enemie_bullets[--(*bullet_counter)];
        }
    }
}

bool are_all_dead(enemie enemies[ENEMIE_ROWS][ENEMIE_COLS]) {
    for (uint32_t i = 0; i < ENEMIE_ROWS; ++i) {
        for (uint32_t j = 0; j < ENEMIE_COLS; ++j) {
            if (!(enemies[i][j].position.x == -1 && enemies[i][j].position.y == -1))
                return false;
        }
    }
    return true;
}

void draw_enemies(enemie enemies[ENEMIE_ROWS][ENEMIE_COLS], Texture2D e_texture, bullet enemie_bullets[ENEMIE_BULLETS], uint32_t bullet_counter) {
    for (uint32_t i = 0; i < ENEMIE_ROWS; ++i) {
        for (uint32_t j = 0; j < ENEMIE_COLS; ++j) {
            if (enemies[i][j].position.x == -1 && enemies[i][j].position.y == -1)
               continue;
            
            DrawTextureV(e_texture, enemies[i][j].position, enemies[i][j].color); 
        }
    }

    for (uint32_t i = 0; i < bullet_counter; ++i) {
        DrawRectangleV(enemie_bullets[i].position, enemie_bullets[i].size, enemie_bullets[i].color);
    }
}
