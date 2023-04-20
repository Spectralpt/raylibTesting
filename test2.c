#include "raylib.h"

void drawMap(Texture2D cubeNoBorder, Texture2D borderDiagonal , Texture2D borderStraight){

  const int screenWidth = 800;
  const int screenHeight = 450;

  int mapHeight = 10;
  int mapWidth = 10;

  int tileWidthHalf = cubeNoBorder.width/2;
  int tileHeightHalf = cubeNoBorder.height/4;
  int cubeHeight = 25;
  int tileScaleFactor = 2;

  BeginDrawing();
    for (int mapY = 0; mapY < mapHeight; mapY++) {
      for (int mapX = 0; mapX < mapWidth; mapX++) {
        Vector2 tilePosition = { (screenWidth/2 - tileWidthHalf*2) + tileScaleFactor*(mapX - mapY) * (tileWidthHalf), ((screenHeight/2) - cubeHeight*tileScaleFactor - (mapHeight*tileHeightHalf)) + tileScaleFactor*(mapX + mapY) * (tileHeightHalf)};  
        DrawTextureEx(cubeNoBorder, tilePosition, 0, tileScaleFactor, WHITE);
        if (mapX == 0 && mapY == mapHeight-1) {
          DrawTextureEx(borderStraight, tilePosition,0, tileScaleFactor, WHITE);
        }
        Vector2 borderPosition = { (screenWidth/2 - tileWidthHalf*2) + tileScaleFactor*(mapX - mapY) * (tileWidthHalf), ((screenHeight/2) - cubeHeight*tileScaleFactor - (mapHeight*tileHeightHalf)) + tileScaleFactor*(mapX + mapY) * (tileHeightHalf)};  
        // Vector2 borderPosition = { 2*tileWidthHalf*tileScaleFactor + (screenWidth/2 - tileWidthHalf*2) + tileScaleFactor*(mapX - mapY) * (tileWidthHalf), 4*tileHeightHalf*tileScaleFactor + ((screenHeight/2) - cubeHeight*tileScaleFactor - (mapHeight*tileHeightHalf)) + tileScaleFactor*(mapX + mapY) * (tileHeightHalf)};  
        // Vector2 borderPosition = { (screenWidth/2 - tileWidthHalf*2) + tileScaleFactor*(mapX - mapY) * (tileWidthHalf) + (2*tileWidthHalf-1)*tileScaleFactor -1,((screenHeight/2) - cubeHeight*tileScaleFactor - (mapHeight*tileHeightHalf)) + tileScaleFactor*(mapX + mapY) * (tileHeightHalf)};  
        if (mapX == mapWidth-1 && mapY == 0) {
          // DrawTextureEx(borderStraight, tilePosition,0, -tileScaleFactor, WHITE);
          DrawTextureEx(borderStraight, borderPosition,0, tileScaleFactor, WHITE);
        }
      }
    }
    for (int mapY = 0; mapY < mapHeight; mapY++) {
      for (int mapX = 0; mapX < (mapWidth+1); mapX++) {
        
        Vector2 tilePosition = { (screenWidth/2 - tileWidthHalf*2) + tileScaleFactor*(mapX - mapY) * (tileWidthHalf), ((screenHeight/2) - cubeHeight*tileScaleFactor - (mapHeight*tileHeightHalf)) + tileScaleFactor*(mapX + mapY) * (tileHeightHalf)};  

        if (mapX == 0) {
          DrawTextureEx(borderDiagonal, tilePosition, 0, tileScaleFactor, WHITE);
        }
      }
    }
    // DrawTexture(borderStraight, 0, (mapHeight*tileHeightHalf), WHITE);
  EndDrawing();
}



int main (void)
{
  
  const int screenWidth = 800;
  const int screenHeight = 450;


  InitWindow(screenWidth, screenHeight, "Test 2");
  SetTargetFPS(60);
  
  Texture2D cubeNoBorder = LoadTexture("../resources/cubeNoBorder.png");
  Texture2D borderDiagonal = LoadTexture("../resources/BorderDiagonal.png");
  Texture2D borderStraight = LoadTexture("../resources/BorderStraight.png");

  while (!WindowShouldClose()) {
    
    ClearBackground((Color){158, 231, 215, 255});
    drawMap(cubeNoBorder, borderDiagonal, borderStraight);
  
    if (IsKeyPressed(KEY_E)) {
      CloseWindow();
    }
  }

  CloseWindow();
  return 0;
}
