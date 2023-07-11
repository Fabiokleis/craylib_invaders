#include <stdint.h>
#include "raylib.h"
#include "bullet.h"
#include "player.h"


bullet Bullet(Vector2 pos, Vector2 size, Vector2 vel, Color color) {
    return (bullet){
      .position = pos,
      .size = size,
      .velocity = vel,
      .color = color,
    };
}

