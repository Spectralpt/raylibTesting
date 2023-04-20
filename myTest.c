#include "raylib.h"
#include "stdio.h"

typedef enum GameScreen {START, GAME} GameScreen;

// #define tile_width_half  16;
// #define tile_height_half  8;

void drawCube(Texture2D topWithBorder, Texture2D topBorderLeft, Texture2D topBorderRight, Texture2D topNoBorder, Texture2D cubeBottomLeft, Texture2D cubeBottomRight, Texture2D cubeFaceCornerLeft, Texture2D cubeFaceCornerRight){
  

  const int tileX = 32;
  const int tileY = 16;

  int tile_width_half = 16;
  int tile_height_half = 8;

  const int screenWidth = 800;
  const int screenHeight = 450;

  int boardWidth = screenWidth/2 - 32;
  int boardHeight = screenHeight/2 - (10*16) - 18;

  int boardPosX = 0;
  int boardPosY = 0;


  int tileScaleFactor = 2;

  for (int mapY = 0; mapY < 10; mapY++) {
    for (int mapX = 0; mapX < 10; mapX++) {

      boardPosX = tileScaleFactor*mapX * tile_width_half - tileScaleFactor*mapY * tile_width_half; 
      boardPosY = tileScaleFactor*mapX * tile_height_half + tileScaleFactor*mapY * tile_height_half;

      int truePosX = boardWidth + boardPosX;
      int truePosY = boardHeight + boardPosY;

      // printf("%i", mapX * tile_width_half);
      // printf("MapX:%i MapY:%i boardPosX%i boardPosY:%i\n", mapX, mapY, boardPosX, boardPosY);
      // printf("%i\n", boardPosY);
      Vector2 pos = {truePosX, truePosY};
      DrawTextureEx(topNoBorder, pos, 0, tileScaleFactor, WHITE);

      if (mapX == 0) {
        DrawTextureEx(topBorderLeft, pos, 0, tileScaleFactor, WHITE);
      }
      if (mapY == 0) {
        DrawTextureEx(topBorderRight, pos, 0, tileScaleFactor, WHITE);
      }
      if (mapX == 0 && mapY == 0) {
        DrawTextureEx(topWithBorder, pos, 0, tileScaleFactor, WHITE);
      }
      if (mapX == 9) {
        DrawTextureEx(cubeBottomRight, pos, 0, tileScaleFactor, WHITE);
      }
      if (mapY == 9) {
        DrawTextureEx(cubeBottomLeft, pos, 0, tileScaleFactor, WHITE); 
      }
      if (mapY == 9 && mapX == 0) {
        DrawTextureEx(cubeFaceCornerLeft, pos, 0, tileScaleFactor, WHITE);
      }
      if (mapX == 9 && mapY == 0) {
        DrawTextureEx(cubeFaceCornerRight, pos, 0, tileScaleFactor, WHITE);
      }
    }
  }
  // DrawTextureEx(texture, pos, 0, 0, WHITE)

  // DrawTexture(texture, boardPosX, boardPosY, WHITE);
  // DrawTexture(texture, 0, 0, WHITE);
  // DrawTexture(texture, 16, 8, WHITE);
  // DrawTexture(texture, 80, 8, WHITE);
  // DrawTexture(texture, 32, 16, WHITE);
  // DrawTexture(texture, 64, 16, WHITE);
  // DrawTexture(texture, 96, 16, WHITE);
  // DrawTexture(texture, 48, 24, WHITE);

}


void drawStart(){
  int posT = 400 - MeasureText("GOOSE GAME", 60)/2;
  int posP = 400 - MeasureText("Play (p)", 30)/2;
  int posS = 400 - (MeasureText("Settings (s)", 30)/2);
  int posE = 400 - MeasureText("Exit (e)", 30)/2;
  BeginDrawing();
  ClearBackground(DARKGRAY);
  DrawText("GOOSE GAME", posT, 100, 60, (Color){255, 246, 128,255});
  DrawText("Play (p)", posP, 200, 30, (Color){109, 200, 98, 255});
  DrawText("Settings (s)", posS, 250, 30, (Color){158, 231, 215, 255});
  DrawText("Exit (e)", posE, 300, 30, (Color){255, 109, 120, 255});
  EndDrawing();
}

void drawGame(Texture2D texture, Texture2D texture2, Texture2D texture3, Texture2D texture4, Texture2D texture5, Texture2D texture6, Texture2D texture8, Texture2D texture9){

  // int postC = 400 - MeasureText("Nice COCK", 80)/2;
  // int postM = 400 - MeasureText("Press (m) to go back to the menu", 30)/2;
  // int postE = 400 - MeasureText("Press (e) to exit", 30)/2;

  BeginDrawing();
  // // ClearBackground(YELLOW);
  ClearBackground((Color){158, 231, 215, 255});
  // DrawText("NICE COCK", postC, 150, 80, (Color){88, 137, 162, 255});
  // DrawText("Press (m) to go back to the menu", postM, 250, 30, DARKGRAY);
  // DrawText("Press (e) to exit", postE, 350, 30, DARKGRAY);

  drawCube(texture, texture2, texture3, texture4, texture5, texture6, texture8, texture9);

  EndDrawing();
}


int main (void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  InitWindow(screenWidth, screenHeight, "My Test Window");
    
  Texture2D topWithBorder = LoadTexture("../resources/cubeFace1-2.png");  
  Texture2D topNoBorder = LoadTexture("../resources/cubeFaceNoBorder.png");
  Texture2D topBorderLeft = LoadTexture("../resources/cubeFaceBorderLeft.png");
  Texture2D topBorderRight = LoadTexture("../resources/cubeFaceBorderRight.png");
  Texture2D cubeBottomLeft = LoadTexture("../resources/cubeFaceBottomLeft.png");
  Texture2D cubeBottomRight = LoadTexture("../resources/cubeFaceBottomRight.png");
  Texture2D cubeFaceCornerLeft = LoadTexture("../resources/cubeFaceCornerLeft.png");
  Texture2D cubeFaceCornerRight = LoadTexture("../resources/cubeFaceCornerRight.png");

  GameScreen currentScreen = START;

  int framesCounter = 0;

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    
    switch (currentScreen) {
      case START:
        drawStart();
        if (IsKeyPressed(KEY_P)) {
          currentScreen = GAME;
        }
        if (IsKeyPressed(KEY_E)) {
          CloseWindow(); 
        }
        break;
      case GAME:
        drawGame(topWithBorder, topBorderLeft, topBorderRight,topNoBorder, cubeBottomLeft, cubeBottomRight, cubeFaceCornerLeft, cubeFaceCornerRight);
        if (IsKeyPressed(KEY_E)) {
          CloseWindow();
        }
        if (IsKeyPressed(KEY_M)) {
          currentScreen = START; 
        }
        break;
    }  
  }
  
  CloseWindow();

  return 0;
}
