#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct polygon{
  Vector2 vertices[4];
}Polygon ;

typedef enum{
  Empty = 0,
  Grass = 1,
  Water = 2,
  Sand = 3,
}Sprite;

void DrawPolygon(Polygon poly){

    DrawCircleV(poly.vertices[0], 2,YELLOW);
    DrawCircleV(poly.vertices[1], 2,RED);
    DrawCircleV(poly.vertices[2], 2,BLUE);
    DrawCircleV(poly.vertices[3], 2,PINK);

    DrawLineV(poly.vertices[0], poly.vertices[1], WHITE);
    DrawLineV(poly.vertices[1], poly.vertices[2], WHITE);
    DrawLineV(poly.vertices[2], poly.vertices[3], WHITE);
    DrawLineV(poly.vertices[3], poly.vertices[0], WHITE);

}

Polygon createSpritePolygon(int tileX, int tileY, Texture2D cube , int tileScaleFactor) {
    Polygon poly;

    poly.vertices[0] = (Vector2){ tileX + (cube.width/2)*(tileScaleFactor), tileY + 1 * tileScaleFactor};
    poly.vertices[1] = (Vector2){ tileX + (cube.width - 1)*tileScaleFactor, tileY + 8 * tileScaleFactor};
    poly.vertices[2] = (Vector2){ tileX + (cube.width/2)*(tileScaleFactor), tileY + (cube.height/2 - 1)*(tileScaleFactor)};
    poly.vertices[3] = (Vector2){ tileX + 1 * tileScaleFactor, tileY + 8 * tileScaleFactor};

    return poly;
}

bool polyPoint(Polygon polygon, float pointX, float pointY) {
  bool collision = false;

  // go through each of the vertices, plus
  // the next vertex in the list
  int next = 0;
  for (int current=0; current<4; current++) {

    // get next vertex in list
    // if we've hit the end, wrap around to 0
    next = current+1;
    if (next == 4) next = 0;

    // get the Vectors at our current position
    // this makes our if statement a little cleaner
    Vector2 vc = polygon.vertices[current];    // c for "current"
    Vector2 vn = polygon.vertices[next];       // n for "next"

    // compare position, flip 'collision' variable
    // back and forth
    if (((vc.y >= pointY && vn.y < pointY) || (vc.y < pointY && vn.y >= pointY)) &&
         (pointX < (vn.x-vc.x)*(pointY-vc.y) / (vn.y-vc.y)+vc.x)) {
            collision = !collision;
    }
  }
  return collision;
}

void drawCube(Texture2D *cubes , int frames, int **heightMutationMap, int **map){

  const int screenWidth = 800;
  const int screenHeight = 450;

  int mapHeight = 9;
  int mapWidth = 9;

  int tileScaleFactor = 2;
  
  Vector2 mouse = GetMousePosition();

  DrawText(TextFormat("x:%f y:%f" , mouse.x, mouse.y), 275, 400, 20, LIGHTGRAY);

  int cubeWidth = cubes[Grass].width;
  
  BeginDrawing();
  for (int mapX = 0; mapX < mapHeight; mapX++) {
    for (int mapY = 0; mapY < mapWidth ; mapY++) {

      int x = (mapX* 0.5 * cubeWidth + mapY * (-0.5) * cubeWidth - cubeWidth/2) * tileScaleFactor + screenWidth/2;
      int y = (mapX* 0.25 * cubeWidth + mapY * 0.25 * cubeWidth) * tileScaleFactor + heightMutationMap[mapX][mapY] * tileScaleFactor;

      Vector2 waterLevel = {0 ,(4 - heightMutationMap[mapX][mapY]) * tileScaleFactor};

      Polygon spritePolygon = createSpritePolygon(x, y, cubes[Grass] ,tileScaleFactor);

      bool collision = polyPoint(spritePolygon, mouse.x, mouse.y);

      Vector2 raisedPosition = {x, collision ? y - 3 * tileScaleFactor : y};

      Color cubeColor = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && collision ? RED : WHITE;
      if (map[mapY][mapX] == Grass) {
        DrawTextureEx(cubes[Grass], raisedPosition, 0, tileScaleFactor, WHITE);  
      }
      if (map[mapY][mapX] == Water) {
        waterLevel = Vector2Add(raisedPosition, waterLevel);
        DrawTextureEx(cubes[Water],  waterLevel, 0, tileScaleFactor, WHITE);
      }
    }
  }
  EndDrawing();
}

int** randomMap(int mapWidth, int mapHeight){
  int **map;

  map = (int**)malloc(mapHeight*sizeof(int*));

  for (int i = 0; i < mapHeight; i++) {
    map[i] = (int*)malloc(mapHeight*sizeof(int));
  }

  for (int mapX = 0; mapX < mapWidth; mapX ++) {
    for (int mapY = 0; mapY < mapHeight; mapY ++) {
      map[mapX][mapY] = (rand() % 2) + 1;
    }
  }
  return map;
}

int** heightMutator(int mapWidth, int mapHeight){

  int** heightMutationMap = (int**)malloc(mapWidth * sizeof(int*));
  for(int i = 0; i < mapWidth; i++){
    heightMutationMap[i] = (int*)malloc(mapHeight * sizeof(int));
  }

  for(int i = 0; i < 9; i++){
      for(int j = 0; j < 9; j++){
          heightMutationMap[i][j] = rand() % 4;
      }
  }
  return heightMutationMap;
}

Texture2D* textureLoader(){

  FILE *texturesFile;
  char line[50];
  int numLines = 3;

  texturesFile =  fopen("sprites/textures.txt", "r");
  if (texturesFile == NULL) {
    printf("Get Fucked");
    exit(1);
  }

  Texture2D *textures = (Texture2D*)malloc(numLines * sizeof(Texture2D));

  for(int i = 0; i < numLines; i++){
  fgets(line, 50, texturesFile);
  line[strcspn(line, "\r\n")] = 0; // Remove newline characters
  textures[i] = LoadTexture(line); // Load texture from line
  }

  fclose(texturesFile);

  return textures;
}

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  srand(time(NULL));

  InitWindow(screenWidth, screenHeight, "My Test 3");
  SetTargetFPS(60);
  int framesCounter = 0;

  int mapWidth = 9, mapHeight = 9;
  int **heightMutationMap = heightMutator(mapWidth, mapHeight);

  Texture2D *textures = (Texture2D*)malloc(3 * sizeof(Texture2D));
  textures = textureLoader();

  int **map = randomMap(mapWidth, mapHeight);

  while (!WindowShouldClose()) {
    ClearBackground((Color){34, 32, 52, 255});
    bool mouseButton = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    drawCube(textures, framesCounter, heightMutationMap, map);
    if(IsKeyPressed(KEY_E)){
      free(textures);
      CloseWindow();
    }
  }
  free(textures);
  CloseWindow();
  return 0;
}


