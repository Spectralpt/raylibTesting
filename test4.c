#include "raylib.h"
#include "raymath.h"

void drawCube(Texture2D cube, Texture2D water){
  const int screenWidth = 800;
  const int screenHeight = 450;

  int mapHeight = 11;
  int mapWidth = 11;

  int tileScaleFactor = 2;

  Vector2 x = {0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};
  Vector2 y = {-0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};



  Vector2 xInverse = {0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};
  Vector2 yInverse = {-0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};

  Vector2 mouse = GetMousePosition();

  // Vector2 x = {0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};
  // Vector2 y = {-0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};

  DrawText(TextFormat("x:%f y:%f" , mouse.x, mouse.y), 275, 400, 20, LIGHTGRAY);
  
  BeginDrawing();
  for (int mapY = 0; mapY < mapHeight; mapY++) {
    for (int mapX = 0; mapX < mapWidth ; mapX++) {
      Vector2 tilePositionXComponent = Vector2Scale(x, mapX);
      Vector2 tilePositionYComponent = Vector2Scale(y, mapY);

      Vector2 inverseTilePositionXComponent = Vector2Scale(inverseTilePositionXComponent, mapX);
      Vector2 inverseTilePositionYComponent = Vector2Scale(inverseTilePositionYComponent, mapY);

      Vector2 tilePosition = Vector2Add(tilePositionXComponent, tilePositionYComponent);
      Vector2 inverseTilePosition = Vector2Add(inverseTilePositionXComponent, inverseTilePositionYComponent);

      int x = (mapX* 0.5 * cube.width + mapY * (-0.5) * cube.width - cube.width/2) * tileScaleFactor + screenWidth/2;
      int y = (mapX* 0.25 * cube.width + mapY * 0.25 * cube.width) * tileScaleFactor;

      Rectangle spriteRectangle = { x + cube.width/2 , y , 0.9*cube.width , 0.6*cube.height };
      bool collision = CheckCollisionPointRec(mouse, spriteRectangle);

      Vector2 raisedPosition = {x, collision ? y - 10 : y + 0};
      DrawTextureEx(cube, raisedPosition, 0, tileScaleFactor, WHITE);
      DrawRectangleLinesEx(spriteRectangle, 2, collision ? RED : BLUE);


      // if (mouse.x == inverseTilePosition.x && mouse.y == inverseTilePosition.y) {
      //   Vector2 raisedPosition = {x, y - 10};
      //   DrawTextureEx(cube, raisedPosition, 0, tileScaleFactor, WHITE);
      // }

      // DrawTextureEx(cube, tilePosition, 0, tileScaleFactor, WHITE);

    }
  }
  EndDrawing();
}


int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "My Test 3");
  SetTargetFPS(60);

  Texture2D cube = LoadTexture("/home/miguel/pictures/Sprites/GooseGame/Sprite-0001.png");
  Texture2D water = LoadTexture("/home/miguel/pictures/Sprites/GooseGame/Sprite-0003.png");
  // Texture2D cube = LoadTexture("../cubes/cube2.png");


  while (!WindowShouldClose()) {
    ClearBackground((Color){34, 32, 52, 255});
    drawCube(cube, water);
    if(IsKeyPressed(KEY_E)){
      CloseWindow();
    }
  }

  CloseWindow();
  return 0;
}


