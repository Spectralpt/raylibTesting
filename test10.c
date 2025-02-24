//This was supposed to be a version of the tile drawer that could handle multiple layers but this gives me brain damage


#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define GRID_SIZE 20;
#define MOVE_SPEED 350.0f;


typedef struct polygon{
  Vector2 vertices[4];
}Polygon ;


typedef enum{
  Empty = 0,
  Grass = 1,
  Water = 2,
  Sand = 3,
}Sprite;


bool colorCompare(Color color1 , Color color2){
  bool sameColor = false;
  if (color1.r == color2.r && color1.g == color2.g && color1.b == color2.b && color1.a == color2.a) {
    return true;
  }
  return false;
}


// Debug function used to visualize de polygons used for collision detection
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


// Creates the poylgons used for collision detection
Polygon createSpritePolygon(int tileX, int tileY, Texture2D cube , int tileScaleFactor) {
    Polygon poly;

    poly.vertices[0] = (Vector2){ tileX + (cube.width/2)*(tileScaleFactor), tileY + 1 * tileScaleFactor};
    poly.vertices[1] = (Vector2){ tileX + (cube.width - 1)*tileScaleFactor, tileY + 8 * tileScaleFactor};
    poly.vertices[2] = (Vector2){ tileX + (cube.width/2)*(tileScaleFactor), tileY + (cube.height/2 - 1)*(tileScaleFactor)};
    poly.vertices[3] = (Vector2){ tileX + 1 * tileScaleFactor, tileY + 8 * tileScaleFactor};

    return poly;
}


// Collision detection formula for point in a polygon
bool polyPoint(Polygon polygon, Vector2 point) {
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
    if (((vc.y >= point.y && vn.y < point.y) || (vc.y < point.y && vn.y >= point.y)) &&
         (point.x < (vn.x-vc.x)*(point.y-vc.y) / (vn.y-vc.y)+vc.x)) {
            collision = !collision;
    }
  }
  return collision;
}

// Draws the UI (currently just the hotbar for the editor)
void drawUI(Texture2D *cubes, int selectedBlock, Texture2D hotbar){
  // Rectangle player1 = {0, 0, 150 , 75};
  // DrawRectangleRounded(player1, 0.1,20,(Color){138,235,241,255});
  // DrawText("Player 1: Miguel", 10, 10, 15, GRAY);
  // DrawText("Points: 100", 10, 30, 15, GRAY);

  // for (int i = 1; i < 4; i++) {
  //   int hotbarLen = i * 45;
  //   int hotbarLenPrev = 0;
  //   if (i > 0) {
  //     hotbarLenPrev = (i-1)* 45;
  //   }
  //   Rectangle hotbar = {GetScreenWidth()/2 + hotbarLen - 50, GetScreenHeight() - 60, 50, 50}; 
  //   Vector2 grassPos = {284 + i * 45, 410};
  //   if (selectedBlock == i) {
  //     DrawRectangleRec(hotbar, (Color){ 200, 200, 200, 220 });
  //   }
  //   else if (selectedBlock != i) {
  //     DrawRectangleRec(hotbar, (Color){ 93, 93, 93, 220 });
  //   }
  //   DrawTextureEx(cubes[i], grassPos, 0, 1, WHITE);
  //   DrawRectangleLinesEx(hotbar, 4, GRAY);
  // }
  Vector2 pos = {25,GetScreenHeight()/2 - hotbar.height};
  DrawTextureEx(hotbar, pos, 0, 2, WHITE);
}

int* selectBlock(){
  int* selectedBlock = (int*)malloc(sizeof(int));
  return selectedBlock;
}

// The work horse of this fucking thing if its supposed to be drawn it goes here (function name should be changed in futture to reflect that)
void drawLayer(Texture2D *cubes , int frames, double **heightMutationMap, int ***map, int mapWidth , int mapHeight,Camera2D camera, Vector2 cameraDelta, Color*** spriteColors, int *selectedBlock, Texture2D hotbar){

  const int screenWidth = 800;
  const int screenHeight = 450;

  int tileScaleFactor = 2;
  
  Vector2 mouse = GetMousePosition();

  int cubeWidth = cubes[Grass].width;
  int cubeHeight = cubes[Grass].height;
  Camera2D cameraPos;

  int mapDepth = 2;

  int x, y;
  Vector3 test;
  
  
  BeginDrawing();
  BeginMode2D(camera);
  for (int mapZ = 0; mapZ < mapDepth; mapZ++) {
    for (int mapX = 0; mapX < mapHeight; mapX++) {
      for (int mapY = 0; mapY < mapWidth ; mapY++) {

        switch (mapZ) {
          case 0:
            x = (mapX* 0.5 * cubeWidth + mapY * (-0.5) * cubeWidth - cubeWidth/2) * tileScaleFactor + screenWidth/2;
            y = (mapX* 0.25 * cubeWidth + mapY * 0.25 * cubeWidth) * tileScaleFactor + heightMutationMap[mapX][mapY] * tileScaleFactor;
          break;
          case 1:
            x = (mapX* 0.5 * cubeWidth + mapY * (-0.5) * cubeWidth - cubeWidth/2) * tileScaleFactor + screenWidth/2;
            y = (mapX* 0.25 * cubeWidth + mapY * 0.25 * cubeWidth) * tileScaleFactor + heightMutationMap[mapX][mapY] * tileScaleFactor;
            y -= cubeHeight/2 * tileScaleFactor;
          break;
          // case 2:
          //   x = (mapX* 0.5 * cubeWidth + mapY * (-0.5) * cubeWidth - cubeWidth/2) * tileScaleFactor + screenWidth/2;
          //   y = (mapX* 0.25 * cubeWidth + mapY * 0.25 * cubeWidth) * tileScaleFactor + heightMutationMap[mapX][mapY] * tileScaleFactor;
          //   y -= cubeHeight * tileScaleFactor;
          // break;
        }   

        Vector2 waterLevel = {0 ,(4 - heightMutationMap[mapX][mapY]) * tileScaleFactor};

        Polygon spritePolygon;

        if (!(mapZ == 1 && map[mapZ][mapY][mapX] == 0)) {
          spritePolygon = createSpritePolygon(x, y, cubes[Grass] ,tileScaleFactor);
        }

        Vector2 spam = Vector2Add(mouse, cameraDelta);
        bool collision = polyPoint(spritePolygon, spam);

        
        Vector2 raisedPosition = {x, y};
        // Vector2 raisedPosition = {x, collision ? y - 3 * tileScaleFactor: y};

        if (collision && !colorCompare(RED, spriteColors[mapZ][mapY][mapX])) {
          spriteColors[mapZ][mapY][mapX] = LIGHTGRAY;
        }
        // you need to add a check for color if this is red i shouldnt change it (this is a proof of concept in case you actually need it)
        if (!collision && !colorCompare(RED, spriteColors[mapZ][mapY][mapX])) {           
          spriteColors[mapZ][mapY][mapX] = WHITE;
        }

        if (collision) {
          test.x = mapX;
          test.y = mapY;
          test.z = mapZ;
        }


        if (collision && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
          spriteColors[mapZ][mapY][mapX] = RED;
        }

        if (map[1][mapY][mapX] == Grass) {
          spriteColors[0][mapY-1][mapX] = RED;
        }

        if (IsKeyPressed(KEY_ONE)) {
          selectedBlock[1] = Grass;
        }
        else if (IsKeyPressed(KEY_TWO)) {
          selectedBlock[1] = Water;
        }
        else if (IsKeyPressed(KEY_THREE)) {
          selectedBlock[1] = Sand;
        }
        else if (IsKeyPressed(KEY_ZERO)) {
          selectedBlock[1] = Empty;
        }
        
        if (collision && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT && mapZ < mapDepth - 1)) {
          map[mapZ+1][mapY][mapX] = selectedBlock[1];
        }

        if (map[mapZ][mapY][mapX] == Grass) {
          DrawTextureEx(cubes[Grass], raisedPosition, 0, tileScaleFactor, spriteColors[mapZ][mapY][mapX]);  
        }
        if (map[mapZ][mapY][mapX] == Sand) {
          DrawTextureEx(cubes[Sand], raisedPosition, 0, tileScaleFactor, spriteColors[mapZ][mapY][mapX]);  
        }
        if (map[mapZ][mapY][mapX] == Water) {
          waterLevel = Vector2Add(raisedPosition, waterLevel);
          DrawTextureEx(cubes[Water],  waterLevel, 0, tileScaleFactor, spriteColors[mapZ][mapY][mapX]);
        }
      }
    }
  }

  EndMode2D();
    // DrawText(TextFormat("x:%f y:%f" , mouse.x, mouse.y), 275, 400, 20, DARKGRAY);
    DrawText(TextFormat("x:%f y:%f z:%f" , test.x, test.y, test.z), 225, 375, 20, DARKGRAY);
  drawUI(cubes, selectedBlock[1], hotbar);
  EndDrawing();
}


// Moves the camera and give the delta for the camera position used to keep the mouse position true to the world coordinates
void cameraMover(Camera2D *camera, Vector2 *mouse){
  
  float deltaTime = GetFrameTime();
  float cameraSpeed = 200;

  if (IsKeyDown(KEY_A)) {
    camera->target.x -= cameraSpeed * deltaTime;
    mouse->x -= cameraSpeed * deltaTime;
  }
  if (IsKeyDown(KEY_D)) {
    camera->target.x += cameraSpeed * deltaTime;
    mouse->x += cameraSpeed * deltaTime;
  }
  if (IsKeyDown(KEY_W)) {
    camera->target.y -= cameraSpeed * deltaTime;
    mouse->y -= cameraSpeed * deltaTime;
  }
  if (IsKeyDown(KEY_S)) {
    camera->target.y += cameraSpeed * deltaTime;
    mouse->y += cameraSpeed * deltaTime;
  }
}


// Test function used as a visual aid deprecated by generateMap()
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


// Allocates the space for a 2D map (3D for now i think)
int*** mapAllocator(int width, int height, int depth){
  int ***map;

  // allocate memory for map
  map = (int***)calloc(depth, sizeof(int**));
  for (int z = 0; z < depth; z++) {
      map[z] = (int**)calloc(height, sizeof(int*));
      for (int i = 0; i < height; i++) {
          map[z][i] = (int*)calloc(width, sizeof(int));
          for (int k = 0; k < width; k++) {
            map[0][i][k] = Grass;
            
          }
      }
  }
  return map;
}


// Generates the a random map (3D) based on a simple generation algorithm 
int*** generateMap(int width, int height) {
    int ***map;
    int i, j, k, l;
    int depth = 2;
    int num_water_tiles = 0;
    int size = width > height ? width : height;
    int max_water_tiles = size * size / 4;

    // seed the random number generator
    srand(time(NULL));

    map = (int***)calloc(depth, sizeof(int**));
    for (int z = 0; z < depth; z++) {
        map[z] = (int**)calloc(height, sizeof(int*));
        for (i = 0; i < height; i++) {
            map[z][i] = (int*)calloc(width, sizeof(int));
        }
    }


    // initialize map to all dirt
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            map[0][i][j] = Grass;
        }
    }

    // add some random water tiles
    for (k = 0; k < 3; k++) {
        i = rand() % height;
        j = rand() % width;
        map[0][i][j] = Water;
        num_water_tiles++;
    }

    // add some more water tiles by spreading out from existing ones
    while (num_water_tiles < max_water_tiles) {
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                if (map[0][i][j] == Water) {
                    if (i > 0 && map[0][i-1][j] != Water && rand() % 2 == 0) {
                        map[0][i-1][j] = Water;
                        num_water_tiles++;
                    }
                    if (i < height-1 && map[0][i+1][j] != Water && rand() % 2 == 0) {
                        map[0][i+1][j] = Water;
                        num_water_tiles++;
                    }
                    if (j > 0 && map[0][i][j-1] != Water && rand() % 2 == 0) {
                        map[0][i][j-1] = Water;
                        num_water_tiles++;
                    }
                    if (j < width-1 && map[0][i][j+1] != Water && rand() % 2 == 0) {
                        map[0][i][j+1] = Water;
                        num_water_tiles++;
                    }
                }
            }
        }
    }

    // add sand around water
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (map[0][i][j] == Water) {
                if (i > 0 && map[0][i-1][j] != Water) {
                    map[0][i-1][j] = Sand;
                }
                if (i < height-1 && map[0][i+1][j] != Water) {
                    map[0][i+1][j] = Sand;
                }
                if (j > 0 && map[0][i][j-1] != Water) {
                    map[0][i][j-1] = Sand;
                }
                if (j < width-1 && map[0][i][j+1] != Water) {
                    map[0][i][j+1] = Sand;
                }
            }
        }
    }
    return map;
}


// Mutates the heigt of the tile by a random ammount to give the tiles some sort of variation
double** heightMutator(int mapWidth, int mapHeight){

  double** heightMutationMap = (double**)malloc(mapWidth * sizeof(double*));
  for(int i = 0; i < mapWidth; i++){
    heightMutationMap[i] = (double*)malloc(mapHeight * sizeof(double));
  }

  for(int i = 0; i < mapWidth; i++){
    printf("\n");
      for(int j = 0; j < mapHeight; j++){
          heightMutationMap[i][j] = rand() % 2;
          heightMutationMap[i][j] *= 32;
          printf("%lf ", heightMutationMap[i][j]);
      }
  }
  return heightMutationMap;
}


// Perlin noise generator, not really sure if we are going to use this though. Can be used as either height heightMutator or map generator
double **generatePerlinNoise(int width, int height, double frequency, int octaves, double persistence)
{
    // allocate memory for the 2D array
    double **noise = (double**)malloc(height * sizeof(double*));
    for (int i = 0; i < height; i++) {
        noise[i] = (double*)malloc(width * sizeof(double));
    }

    // generate the noise grid
    double amplitude = 1.0;
    double total_amplitude = 0.0;
    for (int octave = 0; octave < octaves; octave++) {
        // generate a new grid for this octave
        double **grid = (double**)malloc((width+1) * sizeof(double*));
        for (int i = 0; i < width+1; i++) {
            grid[i] = (double*)malloc((height+1) * sizeof(double));
        }
        for (int i = 0; i < width+1; i++) {
            for (int j = 0; j < height+1; j++) {
                grid[i][j] = (double)rand() / RAND_MAX;
            }
        }

        // interpolate the grid to the target size
        int step_size = width / width;
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                double x = j * step_size * frequency;
                double y = i * step_size * frequency;

                int x0 = floor(x);
                int y0 = floor(y);
                int x1 = x0 + 1;
                int y1 = y0 + 1;

                double u = x - x0;
                double v = y - y0;

                double e00 = grid[x0][y0];
                double e01 = grid[x0][y1];
                double e10 = grid[x1][y0];
                double e11 = grid[x1][y1];

                double a = e00;
                double b = e10 - e00;
                double c = e01 - e00;
                double d = e00 - e10 - e01 + e11;

                noise[i][j] += (a + b*u + c*v + d*u*v) * amplitude;
            }
        }

        // update the frequency and amplitude for the next octave
        frequency *= 2;
        total_amplitude += amplitude;
        amplitude *= persistence;

        // free the grid memory
        for (int i = 0; i < width+1; i++) {
            free(grid[i]);
        }
        free(grid);
    }

    // normalize the noise values to be between 0 and 1
    if (total_amplitude != 0.0) {
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                noise[i][j] /= total_amplitude;
                noise[i][j] = round(noise[i][j] * 31) / 31.0;
                noise[i][j] = 0;
            }
        }
    }

    return noise;
}


// Creates a 3D array that represents the color of the tiles should probably be changed in the future to be included in the map
// as a 3D array of tile structs (block type and color all in one)
Color*** editorColorInit(int mapWidth, int mapHeight, int mapDepth){
  Color ***spriteColors = (Color***)malloc(sizeof(Color**) * mapDepth);
  for (int i = 0; i < mapDepth; i++) {
    spriteColors[i] = (Color**)malloc(sizeof(Color*) * mapWidth);
    for (int j = 0; j < mapWidth; j++) {
      spriteColors[i][j] = (Color*)malloc(sizeof(Color) * mapHeight);
      for (int k = 0; k < mapHeight; k++) {
        spriteColors[i][j][k] = WHITE;
        // if (i == 1) {
        //   spriteColors[i][j][k] = LIGHTGRAY;
        // }
      }
    }
  }
  return spriteColors;
}


// Loads all the textures used in the game and puts them in an array, provides a better way of laoding multiple textures as you can pass them
// all in one array instead of having to pass them individually
Texture2D* textureLoader(){

  FILE *texturesFile;
  char line[50];
  int numLines = 4;

  texturesFile =  fopen("sprites/textures.txt", "r");
  if (texturesFile == NULL) {
    printf("Get Fucked");
    exit(1);
  }

  Texture2D *textures = (Texture2D*)malloc(numLines * sizeof(Texture2D));

  for(int i = 0; i < numLines; i++){
  fgets(line, 50, texturesFile);
  line[strcspn(line, "\r\n")] = 0; // Remove newline characters
  printf("%s\n\n\n\n", line);
  textures[i] = LoadTexture(line); // Load texture from line
  }

  fclose(texturesFile);

  return textures;
}


// The Goat the OG the one and only main function does whatever the fuck you want it to 
int main(void)
{

  // Screen dimentions these will most likely be changed in the future or maybe even set by the user in a menu or maybe even have the game with a
  // set window size but have it scale depending on the resolution
  const int screenWidth = 1920;
  const int screenHeight = 1080;

  // Inition the random number generator
  srand(time(NULL));

  
  // Inition the screen and audio device, default Raylib requirements
  InitWindow(screenWidth, screenHeight, "My Test 3");
  InitAudioDevice();

  Sound music1 = LoadSound("./audio/music/Heartbeat+-+320bit.mp3");
  Sound music2 = LoadSound("./audio/music/Celtic+Ambiance+-+320bit.mp3");

  // PlayMusicStream(music1);

  // Make game run at an expected 60 fps
  SetTargetFPS(60);
  // frame counter may be usefull in the future might aswell define it now
  int framesCounter = 0;

  // map dimentions x, y, z
  int mapWidth = 20, mapHeight = 20, mapDepth = 2;

  // Height mutation maps so for not going to be used to these should be commented out for now (not really cause im lazy and dont want to change 
  // the draw layer function)
  double **heightMutationMap = heightMutator(mapWidth, mapHeight);
  double **heightMutationMap2 = generatePerlinNoise(mapWidth, mapHeight, 0.05, 5, 2);


  // Loads the textures to this arrays using the textureLoader() func
  Texture2D *textures = (Texture2D*)malloc(3 * sizeof(Texture2D));
  textures = textureLoader();

  // Map declaration
  // int **map = randomMap(mapWidth, mapHeight);
  // int ***map = generateMap(mapWidth, mapHeight);
  int ***map = mapAllocator(mapWidth, mapHeight, mapDepth);

  // Camera declaration this should be moved to its own function for the sake of clarity
  Camera2D camera;
  Vector2 position = {0,0};
  camera.zoom = 1.0f;
  camera.offset = position;
  camera.rotation = 0;
  camera.target = position;
  Vector2 cameraDelta = {0 ,0};


  // This is for music pausing i still need to figure that one out
  bool pause = false;


  // Declares the color array base ont he editorColorInit fucn this has mention in the fucntion comment should be changed in the future
  Color ***spriteColors = editorColorInit(mapWidth, mapHeight, mapDepth);

  // for block selection
  int *selectedBlock = selectBlock();

  Texture2D hotbar = LoadTexture("./sprites/hotbar.png");
  // Main game loop this is the GOD of this shit (DO NOT TRY TO CREATE 2 GODS THERE CAN BE ONLY ONE)
  while (!WindowShouldClose()) {
    // UpdateMusicStream(music1);
    ClearBackground((Color){34, 32, 52, 255});
    bool mouseButton = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    Vector2 mouse = GetMousePosition();
    cameraMover(&camera, &cameraDelta);
    // cameraDelta = cameraPosition(&camera);
    drawLayer(textures, framesCounter, heightMutationMap2, map, mapWidth, mapHeight, camera, cameraDelta, spriteColors, selectedBlock, hotbar);

    // If e key is pressed exit game
    if(IsKeyPressed(KEY_E)){
      free(textures);
      CloseWindow();
    }

    // If numpad 1 is pressed play music one
    if(IsKeyDown(KEY_KP_1)){
      PlaySound(music1);
    }
    // If numpad 2 is pressed play music two
    else if (IsKeyPressed(KEY_KP_2)) {
      PlaySound(music2);
    }
    // If numpad 3 is pressed stop all music
    else if (IsKeyPressed(KEY_KP_3)) {

      StopSound(music1);
      StopSound(music2);
    }
  }

  // Free everything before exiting or succumb the the memory gods
  free(textures);
  free(map);
  free(spriteColors);
  CloseWindow();
  return 0;
}


