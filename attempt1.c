// This is the first attempt at a more "working" version of what was created during testingate

#include "raylib.h"
#include "raymath.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


// define a timer
typedef struct{
    float Lifetime;
}Timer;


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
    bool moving;
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


// start or restart a timer with a specific lifetime
void StartTimer(Timer* timer, float lifetime)
{
    if (timer != NULL)
        timer->Lifetime = lifetime;
}


// update a timer with the current frame time
void UpdateTimer(Timer* timer)
{
    // subtract this frame from the timer if it's not allready expired
    if (timer != NULL && timer->Lifetime > 0)
        timer->Lifetime -= GetFrameTime();
}


// check if a timer is done.
bool TimerDone(Timer* timer)
{
    if (timer != NULL)
        return timer->Lifetime <= 0;

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


int GetDisplayWidth() {
    if (IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor();
        return GetMonitorWidth(monitor);
    }
    else {
        return GetScreenWidth();
    }
}


int GetDisplayHeight() {
    if (IsWindowFullscreen()) {
        int monitor = GetCurrentMonitor();
        return GetMonitorHeight(monitor);
    }
    else {
        return GetScreenHeight();
    }
}


Texture2D playerBoard;
Texture2D playerBoard2;
Font boardFont;
// This fucntion will be implemented in the future will be used to draw all the ui or at least call the correspondng fucntiond depending on the current screen
void drawUi(void){

    int width = GetDisplayWidth();
    int height = GetDisplayHeight();
    /* printf("Width:%d;Height:%d\n", width,height); */
    Vector2 boxSize = {width,height};
    float lenght = Vector2Length(boxSize);
    boxSize = Vector2Scale(boxSize, 400/lenght);
    
    Vector2 topLeft = {10 , -5 };
    Vector2 bottomLeft = {0, height - 10 -playerBoard2.height * 2};
    Rectangle bottomRight = {width - playerBoard2.width * 2,  height -10 -playerBoard2.height * 2,playerBoard2.width * 2,playerBoard2.height * 2};
    Vector2 topRight = {width - playerBoard2.width * 2 -10 , -5};
    Rectangle flippedTexture = {0,0,-playerBoard2.width,playerBoard2.height};

    Vector2 zero = {0};
    Vector2 text1Pos = {30,90};
    Vector2 text2Pos = {30,125};
    Rectangle playerBox = {0,0,boxSize.x,boxSize.y};
    DrawTextureEx(playerBoard, topLeft, 0, 2, WHITE);
    DrawTextureEx(playerBoard2, bottomLeft, 0, 2, WHITE);
    DrawTextureEx(playerBoard, topRight, 0, 2, WHITE);
    DrawTexturePro(playerBoard2, flippedTexture, bottomRight, zero, 0, WHITE);
    DrawTextEx(boardFont, "Player 1: Miguel", text1Pos, 30, 1, LIGHTGRAY);
    DrawTextEx(boardFont, "Points: 1000", text2Pos, 30, 1, LIGHTGRAY);
    
    /* DrawText("Player 1", 30, 90, 20, BLACK); */
    /* DrawText("Points: 100", 30 ,125, 20, BLACK); */
}

//Used to store position of currently selected block
SpritePosition* selectBlock(){
    SpritePosition* selectedBlock = (SpritePosition*)malloc(sizeof(SpritePosition));
    return selectedBlock;
}


void drawPlayer(Player player, int tileScaleFactor, int cubeWidth){

    int x = (player.position.x* 0.5 * cubeWidth + player.position.y * (-0.5) * cubeWidth - cubeWidth/2) * tileScaleFactor;
    int y = (player.position.x* 0.25 * cubeWidth + player.position.y * 0.25 * cubeWidth) * tileScaleFactor;

    Vector2 playerScreenPosition = {x, y - (player.texture.height/2 + 5) * tileScaleFactor};

    DrawTextureEx(player.texture, playerScreenPosition, 0, 2, WHITE);
}


Vector2 isometricConversion(Vector2 vector, int tileScaleFactor, int cubeWidth){
    int x = (vector.x * 0.5 + vector.y * (-0.5)) * tileScaleFactor;
    int y = (vector.x * 0.25 + vector.y * 0.25 * cubeWidth) * tileScaleFactor;

    Vector2 vectorIsometric = {x,y};
   return vectorIsometric; 
}


// code made by blizzard engineer by far the best pice of code her
bool MoveTo(Vector2* current, Vector2 target, float speed)
{
    float moveLen = speed * GetFrameTime();

    Vector2 delta = Vector2Subtract(target, *current);
    float lenght = Vector2Length(delta);

    if (lenght < moveLen)
    {
        *current = target;
        return false;
    }

    delta = Vector2Scale(delta, moveLen / lenght);

    *current = Vector2Add(*current, delta);
    return true;
}


void findCameraPositions(int width, int height){
    // Calculate the indices for the centers of each quadrant
    int centerRowTL = width / 2;
    int centerColumnTL = height / 2;

    int centerRowTR = width / 2;
    int centerColumnTR = (height / 2) + (height % 2);

    int centerRowBL = (width / 2) + (width % 2);
    int centerColumnBL = height / 2;

    int centerRowBR = (width / 2) + (width % 2);
    int centerColumnBR = (height / 2) + (height % 2);

    /* // Access the elements at the calculated indices to obtain the centers of each quadrant */
    /* int centerTL = array[centerRowTL][centerColumnTL]; */
    /* int centerTR = array[centerRowTR][centerColumnTR]; */
    /* int centerBL = array[centerRowBL][centerColumnBL]; */
    /* int centerBR = array[centerRowBR][centerColumnBR]; */

}


// Moves the camera to the specified screen coordinates
void cameraPlacer(Camera2D *camera, Vector2* position){
    camera->target.x = position->x;
    camera->target.y = position->y;
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


Vector2 endPosition;
// This function will be used to draw all the game components going from map to players and UI elements
void drawSceneGame(Texture2D *textures,Player *players, Tile **map, int mapWidth, int mapHeight, Camera2D camera, Vector2 cameraDelta, SpritePosition *selectedBlock, int tileScaleFactor){
    int cubeWidth = textures[Grass].width;
    int cubeHeight = textures[Grass].height;
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
            /* TESTING */
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && mouseColision) {
                endPosition.x = mapX;
                endPosition.y = mapY;
                players[0].moving = true;
            }
            if (players[0].moving == true) {
                players[0].moving = MoveTo(&players[0].position, endPosition, 0.01);
            }
            drawPlayer(players[0], tileScaleFactor, cubeWidth);
            /* TESTING */
        }
    }

    EndMode2D();
            drawUi();
    EndDrawing();
}


void drawSceneEditor(){
    // Code goes here xd
}




Player* playerInit(int playerNumber){
    Player *players = (Player*)malloc(playerNumber*sizeof(Player));
    for (int i = 0; i < playerNumber; i++) {
        players[i].position.x = 0;
        players[i].position.y = 0;
        players[i].id = i;
        players[i].moving = false;
    }
    return players;
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


void ToggleFullscreenWindow(int screenWidth, int screenHeight){

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
    SetConfigFlags(FLAG_WINDOW_RESIZABLE || FLAG_FULLSCREEN_MODE);
    InitWindow(screenWidth, screenHeight, "attempt1");
    InitAudioDevice();
    boardFont = LoadFont("./sprites/BLKCHCRY.TTF");
    playerBoard = LoadTexture("./sprites/PlayerBoard2.png"); 
    playerBoard2 = LoadTexture("./sprites/PlayerBoard3.png"); 

    // will try to fix the sounds (aka use music type) in the future for now they wont be used
    //  Sound music1 = LoadSound("./audio/music/Heartbeat+-+320bit.mp3");
    //  Sound music2 = LoadSound("./audio/music/Celtic+Ambiance+-+320bit.mp3");

    // Make game run at an expected 60 fps
    SetTargetFPS(144);
    // frame counter may be usefull in the future might aswell define it now
    int framesCounter = 0;

    // map dimentions x, y, z
    int mapWidth = 40, mapHeight = 40, mapDepth = 2;

    // Loads the textures to this arrays using the textureLoader() func
    Texture2D *textures;// = (Texture2D*)malloc(3 * sizeof(Texture2D));
    textures = textureLoader();

    // Map declaration
    Tile **map = generateMap(mapWidth, mapHeight);

    //
    Player *players = playerInit(1);
    players[0].texture = LoadTexture("./sprites/GOOSESPRITE.png");

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
        /* SetWindowPosition(0,0); */
        // UpdateMusicStream(music1);
        ClearBackground((Color){34, 32, 52, 255});
        bool mouseButton = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        Vector2 mouse = GetMousePosition();
        cameraMover(&camera, &cameraDelta);
        // cameraDelta = cameraPosition(&camera);
        drawSceneGame(textures, players ,map, mapWidth, mapHeight, camera, cameraDelta, selectedBlock, tileScaleFactor);

        // If e key is pressed exit game
        if(IsKeyPressed(KEY_E)){
            free(textures);
            CloseWindow();
        }

        if (IsKeyPressed(KEY_SPACE)) {
            ToggleFullscreenWindow(screenWidth, screenHeight); 
        }
    }

    // Free everything before exiting or succumb the the memory gods
    free(textures);
    free(map);
    CloseWindow();
    return 0;
}

