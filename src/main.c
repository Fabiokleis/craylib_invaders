#include <stdint.h>
#include <raylib.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include "config.h"
#include "player.h"
#include "enemie.h"

void setup(player *p, enemie enemies[ENEMIE_ROWS][ENEMIE_COLS]) {
    InitWindow(WIDTH, HEIGHT, "craylib_invaders");
    InitAudioDevice();              // Initialize audio device
    Sound b_sound = LoadSound("assets/player_bullet_sound.mp3");
    *p = Player(
      (Vector2){(float)WIDTH/2 - 30.0f, (float)HEIGHT - 30.0f},
      (Vector2){30.0f, 30.0f}, (Vector2){400.0f, 0.0f}, BLUE,
      b_sound
    );

    srand(time(NULL));
    for (uint32_t i = 0; i < ENEMIE_ROWS; ++i) {
        for (uint32_t j = 0; j < ENEMIE_COLS; ++j) {
            Vector2 pos = {90.0f*(float)i, 90.0f*(float)j};
            Vector2 size = {45.0f, 45.0f};
            Vector2 vel = {75.0f, 15.0f};
            enemies[i][j] = Enemie(pos, size, vel, PURPLE, (uint32_t)(1 + rand() % ENEMIE_SHOOT_TIME));
        }
    }
}

void update(
    player *p,
    enemie enemies[ENEMIE_ROWS][ENEMIE_COLS],
    bullet enemie_bullets[ENEMIE_BULLETS],
    uint32_t *bullet_counter,
    float delta_time,
    double *bullet_time,
    double *enemie_shoot_time,
    int direction_step
) {
    update_player(p, bullet_time, delta_time);
    update_bullets(p, delta_time);
    update_enemies(p, enemies, enemie_bullets, bullet_counter, enemie_shoot_time, delta_time, direction_step);
}

void draw(player p, enemie enemies[ENEMIE_ROWS][ENEMIE_COLS], bullet enemie_bullets[ENEMIE_BULLETS], uint32_t bullet_counter) {
    BeginDrawing();
    {
        ClearBackground(BLACK);
        draw_enemies(enemies, enemie_bullets, bullet_counter);
        draw_player(p);
        draw_bullets(p);
    }      
    EndDrawing();
}

int main(void) {

 reset:
    double start_time = GetTime();
    double current_time = 0.0;
    double wait_time = 0.0;
    double bullet_time = 0.0;
    double enemie_shoot_time = 0.0;
    float delta_time = 0.0f;
    float time_counter = 0.0f;
    bool pause_flag = false;
    bool killall = false;
    char *time = (char*)malloc(sizeof(uint32_t));

    int direction_step = 1;
    uint32_t bullet_counter = 0;
    
    player p = {0};
    enemie enemies[ENEMIE_ROWS][ENEMIE_COLS];
    bullet enemie_bullets[ENEMIE_BULLETS];

    setup(&p, enemies);
    
    while (!WindowShouldClose() && p.life > 0) {
        PollInputEvents();

        if (killall)
            goto reset;
        
        if (pause_flag) {            
            start_time = 0;
            if (IsKeyPressed(KEY_P))
                pause_flag = false;
        } else {
            if (IsKeyPressed(KEY_P)) {
                pause_flag = true;
            }

            if (are_all_dead(enemies))
                killall = true;
                
            time_counter += delta_time;
            bullet_time += delta_time;
            enemie_shoot_time += delta_time;


            if ((int)time_counter % 2 == 0) {
                direction_step = 1;
            } else {
               direction_step = -1;
            }

            update(&p, enemies, enemie_bullets, &bullet_counter, delta_time, &bullet_time, &enemie_shoot_time, direction_step);

            snprintf(time, sizeof(float), "%u", (uint32_t)time_counter);
            DrawText(time, WIDTH/2, 5, 30, LIGHTGRAY);
            draw(p, enemies, enemie_bullets, bullet_counter);

            SwapScreenBuffer();         // flip the back buffer to screen (front buffer)

            current_time = GetTime();
            wait_time = (FRAME_TIME) - (current_time - start_time);
            if (wait_time > 0.0 && wait_time <= FRAME_TIME) {
                WaitTime((float)wait_time);
                delta_time = (float)(current_time - start_time);
            }
            start_time = current_time;
       }     
    }
    
    CloseWindow();

    return 0;
}
