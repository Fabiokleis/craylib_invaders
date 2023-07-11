#include <stdint.h>
#include "raylib.h"
#include "config.h"

uint32_t FPS = 120;
uint32_t WIDTH = 1200;
uint32_t HEIGHT = 760;

#include "player.h"


int main(void) {

    InitWindow(WIDTH, HEIGHT, "craylib_invaders");

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
