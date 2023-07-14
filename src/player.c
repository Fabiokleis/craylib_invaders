#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include "player.h"
#include "bullet.h"
#include "config.h"


player Player(Vector2 pos, Vector2 size, Vector2 vel, Color color) {
    return (player){
      .position = pos,
      .size = size,
      .velocity = vel,
      .color = color,
      .score = 0,
      .max_bullets = 5,
      .amount_of_bullets = 0,
    };
}

void shoot(player *p) {
    if (p->amount_of_bullets < p->max_bullets) {
        Vector2 b_size = {p->size.x/3, p->size.y/2};
        Vector2 b_pos = {p->position.x + p->size.x/2 - b_size.x/2, p->position.y};
        Vector2 b_vel = {0.0f, 800.0f};
        p->bullets[p->amount_of_bullets++] = Bullet(b_pos, b_size, b_vel, RED);
    }
}

void update_player(player *p, double *bullet_time, float delta_time) {

    if (IsKeyDown(KEY_RIGHT)) {
        if (p->position.x < WIDTH - p->size.x) p->position.x += (p->velocity.x * delta_time);
    }
    
    if (IsKeyDown(KEY_LEFT)) {
        if (p->position.x > 0) p->position.x -= (p->velocity.x * delta_time);
    }
    
    if (IsKeyDown(KEY_UP) && *bullet_time >= 0.2f) {
        shoot(p);
        *bullet_time = 0.0f;
    }
}


void draw_player(player p) {
    DrawRectangleV(p.position, p.size, p.color);

    char *bullets = (char*)malloc(9 + sizeof(uint32_t));
    snprintf(bullets, 9 + sizeof(uint32_t), "bullets: %u", p.amount_of_bullets);
    char *score = (char*)malloc(6 + sizeof(uint32_t));
    snprintf(score, 6 + sizeof(uint32_t), "score: %u", p.score);

    DrawText(score, 10, 10, 20, LIGHTGRAY);
    DrawText(bullets, 10, 30, 20, LIGHTGRAY);
}

void update_bullets(player *p, float delta_time) {
    for (uint32_t i = 0; i < p->amount_of_bullets; ++i) {
        p->bullets[i].position.y -= (p->bullets[i].velocity.y * delta_time);
        
        if (p->bullets[i].position.y <= 0) {
            p->bullets[i] = p->bullets[--(p->amount_of_bullets)];
        }
    }
}

void draw_bullets(player p) {
    for (uint32_t i = 0; i < p.amount_of_bullets; ++i) {
        DrawRectangleV(p.bullets[i].position, p.bullets[i].size, p.bullets[i].color);
    }
}
