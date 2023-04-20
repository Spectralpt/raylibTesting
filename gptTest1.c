#include "raylib.h"

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Tilemap Collision");

    // Load sprite texture
    Texture2D sprite = LoadTexture("/home/miguel/pictures/Sprites/GooseGame/Sprite-0001.png");

    // Set sprite size
    int spriteWidth = sprite.width;
    int spriteHeight = sprite.height;

    // Calculate number of rows and columns in the grid
    int numColumns = screenWidth / spriteWidth;
    int numRows = screenHeight / spriteHeight;

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        // Get mouse cursor position
        Vector2 mousePos = GetMousePosition();

        // Iterate over the grid of sprites
        for (int row = 0; row < numRows; row++)
        {
            for (int col = 0; col < numColumns; col++)
            {
                // Calculate sprite position based on grid position
                Vector2 spritePosition = { col * spriteWidth, row * spriteHeight };

                // Create sprite rectangle for collision detection
                Rectangle spriteRec = { spritePosition.x, spritePosition.y, spriteWidth, spriteHeight };

                // Check collision between mouse cursor and sprite rectangle
                bool collision = CheckCollisionPointRec(mousePos, spriteRec);

                // Draw sprite
                DrawTextureEx(sprite, spritePosition, 0.0f, 1.0f, WHITE);

                // Draw sprite rectangle (for visualization)
                DrawRectangleLinesEx(spriteRec, 2, collision ? RED : BLUE);
            }
        }

        // Draw
        BeginDrawing();

        ClearBackground(RAYWHITE);

        EndDrawing();
    }

    // Unload texture
    UnloadTexture(sprite);

    // Close window and OpenGL context
    CloseWindow();

    return 0;
}

