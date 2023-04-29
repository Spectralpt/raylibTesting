#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define GRID_SIZE 20;

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

void drawCube(Texture2D *cubes , int frames, double **heightMutationMap, int **map){

  const int screenWidth = 800;
  const int screenHeight = 450;

  int mapHeight = 20;
  int mapWidth = 20;

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
      if (map[mapY][mapX] == Sand) {
        DrawTextureEx(cubes[Sand], raisedPosition, 0, tileScaleFactor, WHITE);  
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

int** generateMap(int width, int height) {
    int **map;
    int i, j, k, l;
    int num_water_tiles = 0;
    int size = width > height ? width : height;
    int max_water_tiles = size * size / 4;

    // seed the random number generator
    srand(time(NULL));

    // allocate memory for map
    map = (int**)malloc(height*sizeof(int*));
    for (i = 0; i < height; i++) {
        map[i] = (int*)malloc(width*sizeof(int));
    }

    // initialize map to all dirt
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            map[i][j] = Grass;
        }
    }

    // add some random water tiles
    for (k = 0; k < 3; k++) {
        i = rand() % height;
        j = rand() % width;
        map[i][j] = Water;
        num_water_tiles++;
    }

    // add some more water tiles by spreading out from existing ones
    while (num_water_tiles < max_water_tiles) {
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                if (map[i][j] == Water) {
                    if (i > 0 && map[i-1][j] != Water && rand() % 2 == 0) {
                        map[i-1][j] = Water;
                        num_water_tiles++;
                    }
                    if (i < height-1 && map[i+1][j] != Water && rand() % 2 == 0) {
                        map[i+1][j] = Water;
                        num_water_tiles++;
                    }
                    if (j > 0 && map[i][j-1] != Water && rand() % 2 == 0) {
                        map[i][j-1] = Water;
                        num_water_tiles++;
                    }
                    if (j < width-1 && map[i][j+1] != Water && rand() % 2 == 0) {
                        map[i][j+1] = Water;
                        num_water_tiles++;
                    }
                }
            }
        }
    }

    // add sand around water
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (map[i][j] == Water) {
                if (i > 0 && map[i-1][j] != Water) {
                    map[i-1][j] = Sand;
                }
                if (i < height-1 && map[i+1][j] != Water) {
                    map[i+1][j] = Sand;
                }
                if (j > 0 && map[i][j-1] != Water) {
                    map[i][j-1] = Sand;
                }
                if (j < width-1 && map[i][j+1] != Water) {
                    map[i][j+1] = Sand;
                }
            }
        }
    }
    return map;
}

int** heightMutator(int mapWidth, int mapHeight){

  int** heightMutationMap = (int**)malloc(mapWidth * sizeof(int*));
  for(int i = 0; i < mapWidth; i++){
    heightMutationMap[i] = (int*)malloc(mapHeight * sizeof(int));
  }

  for(int i = 0; i < mapWidth; i++){
      for(int j = 0; j < mapHeight; j++){
          heightMutationMap[i][j] = rand() % 4;
      }
  }
  return heightMutationMap;
}

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


    // // normalize the noise values to be between 0 and 1
    // for (int i = 0; i < height; i++) {
    //     for (int j = 0; j < width; j++) {
    //         noise[i][j] /= total_amplitude;
    //         noise[i][j] = fmax(0, noise[i][j]);
    //     }
    // }

for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            noise[i][j] /= total_amplitude;
            noise[i][j] *= 5.0;
            noise[i][j] = round(noise[i][j]);
            noise[i][j] = fmax(0.0, noise[i][j]);
        }
    }
    // round the noise values to the nearest integer
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                noise[i][j] = round(noise[i][j]);
            }
        }
    for (int i = 0; i < height; i++) {
        printf("\n");
        for (int j = 0; j < width; j++) {
          printf("%lf", noise[i][j]);
        }
    }
    return noise;
}


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

int main(void)
{
  const int screenWidth = 800;
  const int screenHeight = 450;

  srand(time(NULL));

  InitWindow(screenWidth, screenHeight, "My Test 3");
  SetTargetFPS(60);
  int framesCounter = 0;

  int mapWidth = 20, mapHeight = 20;
  // int **heightMutationMap = heightMutator(mapWidth, mapHeight);
  double **heightMutationMap = generatePerlinNoise(mapWidth, mapHeight, 0.05, 5, 2);

  Texture2D *textures = (Texture2D*)malloc(3 * sizeof(Texture2D));
  textures = textureLoader();

  // int **map = randomMap(mapWidth, mapHeight);
  int **map = generateMap(mapWidth, mapHeight);

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


