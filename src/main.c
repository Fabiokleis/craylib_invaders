#include "raylib.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

uint32_t FPS = 120;
uint32_t WIDTH = 1200;
uint32_t HEIGHT = 760;

typedef struct bullet {
    Vector2 position;
    Vector2 size;
    Vector2 velocity;
    Color color;
} bullet;

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

bullet Bullet(Vector2 pos, Vector2 size, Vector2 vel, Color color) {
    return (bullet){
      .position = pos,
      .size = size,
      .velocity = vel,
      .color = color,
    };
}

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

void draw_bullets(player p) {
    for (uint32_t i = 0; i < p.amount_of_bullets; ++i) {
        DrawRectangleV(p.bullets[i].position, p.bullets[i].size, p.bullets[i].color);
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
        if (p->position.x > 1) p->position.x -= (p->velocity.x * delta_time);
    }
    
    if (IsKeyDown(KEY_UP) && *bullet_time >= 0.2f) {
        shoot(p);
        *bullet_time = 0.0f;
    }
}

void update_bullets(player *p, float delta_time) {
    for (uint32_t i = 0; i < p->amount_of_bullets; ++i) {
        p->bullets[i].position.y -= (p->bullets[i].velocity.y * delta_time);
        
        if (p->bullets[i].position.y <= 0) {
            p->bullets[i] = p->bullets[--(p->amount_of_bullets)];
        }
    }
}

int main(void) {

    InitWindow(WIDTH, HEIGHT, "craylib_invaiders");

    double start_time = GetTime();
    double current_time = 0.0;
    double wait_time = 0.0;
    double bullet_time = 0.0;
    float delta_time = 0.0f;
    float time_counter = 0.0f;
    

    player p = Player(
      (Vector2){(float)WIDTH/2 - 30.0f, (float)HEIGHT - 30.0f},
      (Vector2){30.0f, 30.0f}, (Vector2){400.0f, 0.0f}, BLUE
    );    
    
    while (!WindowShouldClose()) {
        time_counter += delta_time;
        bullet_time += delta_time;
                
        update_player(&p, &bullet_time, delta_time);
        update_bullets(&p, delta_time);
          
        PollInputEvents();

        
        BeginDrawing();
            ClearBackground(DARKGRAY);
                
            draw_player(p);
            draw_bullets(p);
           
        EndDrawing();
        
        SwapScreenBuffer();         // flip the back buffer to screen (front buffer)
        
        current_time = GetTime();
        wait_time = (1.0f/(float)FPS) - (current_time - start_time);
        if (wait_time > 0.0) {
            WaitTime((float)wait_time);
            delta_time = (float)(current_time - start_time);
        }
        start_time = current_time;
    }
    

    CloseWindow();

    return 0;
}
