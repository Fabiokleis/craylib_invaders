#include <raylib.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include "player.h"
#include "bullet.h"
#include "config.h"


player Player(Vector2 pos, Vector2 size, Vector2 vel, Color color, Sound bullet_sound, Texture2D p_texture) {
    return (player){
      .position = pos,
      .size = size,
      .velocity = vel,
      .color = color,
      .score = 0,
      .max_bullets = 5,
      .amount_of_bullets = 0,
      .life = LIFES,
      .bullet_sound = bullet_sound,
      .p_texture = p_texture,
    };
}

Vector2 sub(Vector2 v0, Vector2 v1) {
    return (Vector2){v0.x - v1.x, v0.y - v1.y};
}

float dot(Vector2 v0, Vector2 v1) {
    return (v0.x * v1.x + v0.y * v1.y);
}

float norma(Vector2 v0) {
    return sqrt(v0.x * v0.x + v0.y * v0.y);
}

Vector2 perpendicular(Vector2 v0) {
    Vector2 axis = {v0.y, -v0.x};
    return (Vector2){axis.x/norma(axis), axis.y/norma(axis)};
}

float sat_collision_detect(Vector2 pos1, Vector2 size1, Vector2 pos2, Vector2 size2) {
    Vector2 va[4], vb[4];
    va[0] = (Vector2){pos1.x, pos1.y};
    va[1] = (Vector2){pos1.x + size1.x, pos1.y};
    va[2] = (Vector2){pos1.x + size1.x, pos1.y + size1.y};
    va[3] = (Vector2){pos1.x, pos1.y + size1.y};

    vb[0] = (Vector2){pos2.x, pos2.y};
    vb[1] = (Vector2){pos2.x + size2.x, pos2.y};
    vb[2] = (Vector2){pos2.x + size2.x, pos2.y + size2.y};
    vb[3] = (Vector2){pos2.x, pos2.y + size2.y};

    
    float separation = -99999.0f;
    for (uint32_t i = 0; i < 4; ++i) {
        Vector2 normal = perpendicular(sub(va[(i + 1) % 4], va[i]));
        float minsep = 99999.0f;

        for (uint32_t j = 0; j < 4; ++j) {
            minsep = fmin(minsep, dot(sub(vb[j], va[i]), normal));   
        }
        
        if (minsep > separation) {
            separation = minsep;
        }
    }
    return separation;
}

bool is_colliding_squares(Vector2 pos1, Vector2 size1, Vector2 pos2, Vector2 size2) {
    return sat_collision_detect(pos1, size1, pos2, size2) <= 0 &&
        sat_collision_detect(pos2, size2, pos1, size1) <= 0;
}

void shoot(player *p) {
    if (p->amount_of_bullets < p->max_bullets) {
        Vector2 b_size = {p->size.x/3, p->size.y};
        Vector2 b_pos = {p->position.x + p->size.x/2 - b_size.x/2, p->position.y};
        Vector2 b_vel = {0.0f, 800.0f};
        p->bullets[p->amount_of_bullets++] = Bullet(b_pos, b_size, b_vel, VIOLET);
    }
}
    
void update_player(player *p, double *bullet_time, float delta_time) {

    if (IsKeyDown(KEY_RIGHT)) {
        if (p->position.x < WIDTH - p->size.x) p->position.x += (p->velocity.x * delta_time);
    }
    
    if (IsKeyDown(KEY_LEFT)) {
        if (p->position.x > 0) p->position.x -= (p->velocity.x * delta_time);
    }

    if ((IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP)) && *bullet_time >= 0.2f) {
        shoot(p);
        PlaySound(p->bullet_sound);
        *bullet_time = 0.0f;
    }
}


void draw_player(player p) {
    DrawTextureV(p.p_texture, p.position, p.color);

    char *bullets = (char*)malloc(9 + sizeof(uint32_t));
    snprintf(bullets, 9 + sizeof(uint32_t), "bullets: %u", p.amount_of_bullets);
    
    char *score = (char*)malloc(7 + sizeof(uint32_t));
    snprintf(score, 7 + sizeof(uint32_t), "score: %u", p.score);

    char *lifes = (char*)malloc(5 + sizeof(uint32_t));
    snprintf(lifes, 6 + sizeof(uint32_t), "life: %u", p.life);

    DrawText(score, 10, 10, 20, LIGHTGRAY);
    DrawText(bullets, 10, 30, 20, LIGHTGRAY);
    DrawText(lifes, 10, 50, 20, LIGHTGRAY);
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
