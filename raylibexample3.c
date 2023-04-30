#include "raylib.h"

#define MOVE_SPEED 100.0f

int main(void){

  const int screenWidth = 800;
  const int screenHeight = 450;
  Vector2 position = {0,0};

  InitWindow(screenWidth, screenHeight, "HI");
  SetTargetFPS(60);

  Texture2D texture = LoadTexture("sprites/Sprite-0001.png");
  
  Camera2D camera;
  camera.zoom = 1.0f;
  camera.offset.x = 0;
  camera.offset.y = 0;
  camera.rotation = 0;
  camera.target = position;

  // Vector2 spam = {-screenWidth/2,-screenHeight/2};

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    float deltaTime = GetFrameTime();

    // Vector2 spam = GetMousePosition();

    BeginMode2D(camera);
    DrawTextureEx(texture, position, 0, 1,WHITE);
    if (IsKeyDown(KEY_A)) {
      camera.target.x += MOVE_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_D)) {
      camera.target.x -= MOVE_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_W)) {
      camera.target.y += MOVE_SPEED * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
      camera.target.y -= MOVE_SPEED * deltaTime;
    }

    EndMode2D();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}
