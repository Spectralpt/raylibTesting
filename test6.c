#include "raylib.h"
#include "raymath.h"

typedef struct polygon{
  Vector2 vertices[4];
}Polygon ;

typedef enum{
  Empty = 0,
  Grass = 1,
  Water = 2,
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

void drawCube(Texture2D cube, Texture2D water , int frames){

  int map [9][9] = {{0,0,0,1,1,1,0,0,0},
                    {0,0,0,1,1,1,0,0,0},
                    {0,0,0,2,2,1,0,0,0},
                    {0,0,0,2,2,2,0,0,0},
                    {0,0,0,2,2,2,0,0,0},
                    {0,0,0,2,2,2,0,0,0},
                    {0,1,1,1,2,2,2,1,0},
                    {0,1,1,1,2,1,1,1,0},
                    {0,1,1,1,0,1,1,1,0}};

  const int screenWidth = 800;
  const int screenHeight = 450;

  int mapHeight = 9;
  int mapWidth = 9;

  int tileScaleFactor = 2;

  Vector2 x = {0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};
  Vector2 y = {-0.5 * cube.width * tileScaleFactor, 0.25 * cube.height * tileScaleFactor};

  Vector2 mouse = GetMousePosition();

  DrawText(TextFormat("x:%f y:%f" , mouse.x, mouse.y), 275, 400, 20, LIGHTGRAY);
  
  BeginDrawing();
  for (int mapX = 0; mapX < mapHeight; mapX++) {
    for (int mapY = 0; mapY < mapWidth ; mapY++) {

      int x = (mapX* 0.5 * cube.width + mapY * (-0.5) * cube.width - cube.width/2) * tileScaleFactor + screenWidth/2;
      int y = (mapX* 0.25 * cube.width + mapY * 0.25 * cube.width) * tileScaleFactor;

      Vector2 waterLevel = {0 ,5};

      Polygon spritePolygon = createSpritePolygon(x, y, cube ,tileScaleFactor);

      bool collision = polyPoint(spritePolygon, mouse.x, mouse.y);

      Vector2 raisedPosition = {x, collision ? y - 5 : y};
      Color cubeColor = IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && collision ? RED : WHITE;
      if (map[mapY][mapX] == Grass) {
        DrawTextureEx(cube, raisedPosition, 0, tileScaleFactor, WHITE);  
      }
      if (map[mapY][mapX] == Water) {
        waterLevel = Vector2Add(raisedPosition, waterLevel);
        DrawTextureEx(water,  waterLevel, 0, tileScaleFactor, WHITE);
      }
      // if (mapX == mapY) {
      //   DrawTextureEx(water, waterLevel, 0, tileScaleFactor, WHITE);  
      // }
      // if (mapX != mapY) {
      //   DrawTextureEx(cube, raisedPosition, 0, tileScaleFactor, cubeColor);      
      // }
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
  int framesCounter = 0;

  Texture2D cube = LoadTexture("sprites/Sprite-0001.png");
  Texture2D water = LoadTexture("sprites/Sprite-0003.png");
  // Texture2D cube = LoadTexture("../cubes/cube2.png");


  while (!WindowShouldClose()) {
    ClearBackground((Color){34, 32, 52, 255});
    bool mouseButton = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    drawCube(cube, water, framesCounter);
    if(IsKeyPressed(KEY_E)){
      CloseWindow();
    }
  }

  CloseWindow();
  return 0;
}


