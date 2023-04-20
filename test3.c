#include "raylib.h"
#include "raymath.h"

void drawCube(Texture2D cube, Texture2D water){
  const int screenWidth = 800;
  const int screenHeight = 450;

  int mapHeight = 11;
  int mapWidth = 11;

  int tileScaleFactor = 2;
  // int tileWidthHalf = (cube.width/2 - 1) * tileScaleFactor;
  // int tileHeightHalf = (cube.height/4 - 1) * tileScaleFactor;
  int cubeHeight = 25;

  Vector2 x = {0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};
  Vector2 y = {-0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};
  Vector2 inverseTilePositionXComponent = {4 / (cube.width * tileScaleFactor), -4 / (cube.height * tileScaleFactor)};
  Vector2 inverseTilePositionYComponent = {8 / (cube.width * tileScaleFactor), 8 / (cube.height * tileScaleFactor)};
  Vector2 mouse = GetMousePosition();
  
  BeginDrawing();
  for (int mapY = 0; mapY < mapHeight; mapY++) {
    for (int mapX = 0; mapX < mapWidth ; mapX++) {
      Vector2 tilePositionXComponent = Vector2Scale(x, mapX);
      Vector2 tilePositionYComponent = Vector2Scale(y, mapY);


      Vector2 inverseTilePositionXComponent = Vector2Scale(inverseTilePositionXComponent, mapX);
      Vector2 inverseTilePositionYComponent = Vector2Scale(inverseTilePositionYComponent, mapY);
      // int x = (mapX* 0.5 * cube.width + mapY * (-0.5) * cube.width - cube.width/2) * tileScaleFactor + screenWidth/2;
      // int y = (mapX* 0.25 * cube.width + mapY * 0.25 * cube.width) * tileScaleFactor + (screenHeight - (cube.height * mapHeight * tileScaleFactor))/2;

      Vector2 tilePosition = Vector2Add(tilePositionXComponent, tilePositionYComponent);
      Vector2 inverseTilePosition = Vector2Add(inverseTilePositionXComponent, inverseTilePositionYComponent);
      DrawTextureEx(cube, tilePosition, 0, tileScaleFactor, WHITE);
      // Vector2 tilePosition = {x,y};
      // Vector2 tilePosition = {(mapX - mapY) * tileWidthHalf,(mapX + mapY) * tileHeightHalf};
      // Vector2 tilePosition = {(mapX - mapY) * cube.width/2,(mapX + mapY) * cube.height/2};
      // if (mapX < 9 && mapY < 9) {
      // DrawTextureEx(cube, tilePosition, 0, tileScaleFactor, WHITE);
      // }
      // if (mapX >= 9 || mapY >= 9) {
      //   Vector2 waterPosition = {x , y + 20};
      // DrawTextureEx(water, waterPosition, 0, tileScaleFactor, WHITE);
      // }
      if (mouse == inverseTilePosition) {
        
      }
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


