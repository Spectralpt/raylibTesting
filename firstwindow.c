#include "raylib.h"

#define BLUEMINE (Color){158, 232, 216, 255};

int main (void)
{
  const int  screenWidth = 800; 
  const int  screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "my first window");

  SetTargetFPS(60);

  while (!WindowShouldClose()) {

    BeginDrawing();
      ClearBackground(LIGHTGRAY);
      DrawText("Nice window", 350, 200, 20, (Color){158,232,216,255});
    EndDrawing();
  }

  CloseWindow();

  return 0;
}
