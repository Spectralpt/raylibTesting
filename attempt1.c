// This is the first attempt at a more "working" version of what was created during testing

#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


typedef struct spritePosition{
    int x;
    int y;
} SpritePosition;


typedef struct polygon{
    Vector2 vertices[4];
}Polygon ;


typedef struct tile{
    int x;
    int y;
    Color color;
    int type;
}Tile;


typedef struct Player{
    int id;
    Texture2D texture;
    Vector2 position;
} Player;


typedef enum{
    Empty = 0,
    Grass = 1,
    Water = 2,
    Sand = 3,
}Sprite;

//Color compare fucntion used to compare the color type inside raylib
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


// Collision detection formula for point in a polygon we use this to detect if the mouse inside a non-regular polygon (aka sprites)
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


// This fucntion will be implemented in the future will be used to draw all the ui or at least call the correspondng fucntiond depending on the current screen
void drawUi(void){
    // Code goes here xd 
}

//Used to store position of currently selected block
SpritePosition* selectBlock(){
    SpritePosition* selectedBlock = (SpritePosition*)malloc(sizeof(SpritePosition));
    return selectedBlock;
}


void drawPlayer(Player player, int tileScaleFactor, int cubeWidth){

    int x = (player.position.x* 0.5 * cubeWidth + player.position.y * (-0.5) * cubeWidth - cubeWidth/2) * tileScaleFactor;
    int y = (player.position.x* 0.25 * cubeWidth + player.position.y * 0.25 * cubeWidth) * tileScaleFactor;

    Vector2 playerScreenPos = {x, y - (player.texture.height/2 - 10) * tileScaleFactor};

    DrawTextureEx(player.texture, playerScreenPos, 0, 2, WHITE);
}


void playerPostionUpdate(Player player, Vector2 endPosition, int tileScaleFactor, int cubeWidth){
    for (player.position.y;  player.position.y < endPosition.y; player.position.y++) {
        for (player.position.x; player.position.x < endPosition.x; player.position.x++) {
            drawPlayer(player, tileScaleFactor, cubeWidth);
        }
    }
}


// This function will be used to draw all the game components going from map to players and UI elements
void drawSceneGame(Texture2D *textures,Texture2D player, Tile **map, int mapWidth, int mapHeight, Camera2D camera, Vector2 cameraDelta, SpritePosition *selectedBlock, int tileScaleFactor){
    int cubeWidth = textures[Grass].width;
    int cubeHeight = textures[Grass].height;
    //TESTING
    Player player1;
    player1.id = 1;
    player1.position.x = -1;
    player1.position.y = -1;
    player1.texture = player;
    //TESTING
    BeginDrawing();
    BeginMode2D(camera);
    for (int mapX = 0; mapX < mapHeight; mapX++) {
        for (int mapY = 0; mapY < mapWidth ; mapY++) {
            int x = (mapX* 0.5 * cubeWidth + mapY * (-0.5) * cubeWidth - cubeWidth/2) * tileScaleFactor;
            int y = (mapX* 0.25 * cubeWidth + mapY * 0.25 * cubeWidth) * tileScaleFactor;

            Vector2 spritePosition = {x,y};
            Vector2 waterLevel = {0 + x,4 * tileScaleFactor + y};

            Polygon spritePolygon = createSpritePolygon(x, y, textures[Grass] ,tileScaleFactor);
            Vector2 mousePositionCorrection = Vector2Add(GetMousePosition(), cameraDelta);
            bool mouseColision = polyPoint(spritePolygon, mousePositionCorrection);

                drawPlayer(player1, tileScaleFactor, cubeWidth);

            switch (map[mapY][mapX].type) {
                case Grass:
                    DrawTextureEx(textures[Grass], spritePosition, 0, tileScaleFactor, mouseColision ? LIGHTGRAY : WHITE );
                    break;
                case Water:
                    DrawTextureEx(textures[Water], waterLevel, 0, tileScaleFactor, mouseColision ? LIGHTGRAY : WHITE );
                    break;
                case Sand:
                    DrawTextureEx(textures[Sand], spritePosition, 0, tileScaleFactor, mouseColision ? LIGHTGRAY : WHITE );
                    break;
            }
        }
    }

    EndMode2D();
    EndDrawing();
}


void drawSceneEditor(){
    // Code goes here xd
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


// Generates random 2d map based on a simple algorithm  
Tile** generateMap(int width, int height) {
    Tile **map;
    int i, j, k, l;
    int num_water_tiles = 0;
    int size = width > height ? width : height;
    int max_water_tiles = size * size / 4;

    // seed the random number generator
    srand(time(NULL));

    // allocate memory for map
    map = (Tile**)malloc(height*sizeof(Tile*));
    for (i = 0; i < height; i++) {
        map[i] = (Tile*)malloc(width*sizeof(Tile));
    }

    // initialize map to all dirt
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            map[i][j].type = Grass;
        }
    }

    // add some random water tiles
    for (k = 0; k < 3; k++) {
        i = rand() % height;
        j = rand() % width;
        map[i][j].type = Water;
        num_water_tiles++;
    }

    // add some more water tiles by spreading out from existing ones
    while (num_water_tiles < max_water_tiles) {
        for (i = 0; i < height; i++) {
            for (j = 0; j < width; j++) {
                if (map[i][j].type == Water) {
                    if (i > 0 && map[i-1][j].type != Water && rand() % 2 == 0) {
                        map[i-1][j].type = Water;
                        num_water_tiles++;
                    }
                    if (i < height-1 && map[i+1][j].type != Water && rand() % 2 == 0) {
                        map[i+1][j].type = Water;
                        num_water_tiles++;
                    }
                    if (j > 0 && map[i][j-1].type != Water && rand() % 2 == 0) {
                        map[i][j-1].type = Water;
                        num_water_tiles++;
                    }
                    if (j < width-1 && map[i][j+1].type != Water && rand() % 2 == 0) {
                        map[i][j+1].type = Water;
                        num_water_tiles++;
                    }
                }
            }
        }
    }

    // add sand around water
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            if (map[i][j].type == Water) {
                if (i > 0 && map[i-1][j].type != Water) {
                    map[i-1][j].type = Sand;
                }
                if (i < height-1 && map[i+1][j].type != Water) {
                    map[i+1][j].type = Sand;
                }
                if (j > 0 && map[i][j-1].type != Water) {
                    map[i][j-1].type = Sand;
                }
                if (j < width-1 && map[i][j+1].type != Water) {
                    map[i][j+1].type = Sand;
                }
            }
        }
    }
    return map;
}


// Loads all the textures used in the game and puts them in an array, provides a better way of laoding multiple textures as you can pass them
// all in one array instead of having to pass them individually
Texture2D* textureLoader(){

    FILE *texturesFile;
    char line[50];
    int numLines = 4;
    char characterInFile;

    //  for (characterInFile = getc(texturesFile); characterInFile != EOF; characterInFile = getc(texturesFile))
    //      if (characterInFile == '\n') 
    //          numLines = numLines + 1;

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


void FullscreenToggle(int screenWidth, int screenHeight){

    if (!IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor();
        SetWindowSize(GetMonitorWidth(monitor), GetMonitorHeight(monitor));
        ToggleFullscreen();
    }
    else {
        ToggleFullscreen();
        SetWindowSize(screenWidth, screenHeight);
    }
}


// The Goat the OG the one and only main function does whatever the fuck you want it to 
int main(void)
{
    // Screen dimentions these will most likely be changed in the future or maybe even set by the user in a menu or maybe even have the game with a
    // set window size but have it scale depending on the resolution
    const int screenWidth = 1920;
    const int screenHeight = 1080;

    int tileScaleFactor = 2;

    // init the random number generator
    srand(time(NULL));

    // init the screen and audio device, default Raylib requirements
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(screenWidth, screenHeight, "attempt1");
    InitAudioDevice();

    // will try to fix the sounds (aka use music type) in the future for now they wont be used
    //  Sound music1 = LoadSound("./audio/music/Heartbeat+-+320bit.mp3");
    //  Sound music2 = LoadSound("./audio/music/Celtic+Ambiance+-+320bit.mp3");

    // Make game run at an expected 60 fps
    SetTargetFPS(60);
    // frame counter may be usefull in the future might aswell define it now
    int framesCounter = 0;

    // map dimentions x, y, z
    int mapWidth = 20, mapHeight = 20, mapDepth = 2;

    // Loads the textures to this arrays using the textureLoader() func
    Texture2D *textures;// = (Texture2D*)malloc(3 * sizeof(Texture2D));
    textures = textureLoader();
    Texture2D player;
    player = LoadTexture("./sprites/GOOSESPRITE.png");

    // Map declaration
    Tile **map = generateMap(mapWidth, mapHeight);

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

    // for block selection
    SpritePosition *selectedBlock = selectBlock();

    Texture2D hotbar = LoadTexture("./sprites/hotbar.png");
    // Main game loop this is the GOD of this shit (DO NOT TRY TO CREATE 2 GODS THERE CAN BE ONLY ONE)
    while (!WindowShouldClose()) {
        SetWindowPosition(0,0);
        // UpdateMusicStream(music1);
        ClearBackground((Color){34, 32, 52, 255});
        bool mouseButton = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        Vector2 mouse = GetMousePosition();
        cameraMover(&camera, &cameraDelta);
        // cameraDelta = cameraPosition(&camera);
        drawSceneGame(textures,player, map, mapWidth, mapHeight, camera, cameraDelta, selectedBlock, tileScaleFactor);

        // If e key is pressed exit game
        if(IsKeyPressed(KEY_E)){
            free(textures);
            CloseWindow();
        }

        if (IsKeyPressed(KEY_SPACE)) {
            FullscreenToggle(screenWidth, screenHeight); 
        }
    }

    // Free everything before exiting or succumb the the memory gods
    free(textures);
    free(map);
    CloseWindow();
    return 0;
}
