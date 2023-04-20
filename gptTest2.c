#include "raylib.h"

int main(void)
{
    // Initialization
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Isometric Tilemap Collision");

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

        // Convert mouse position to isometric tile position
        int mouseX = mousePos.x;
        int mouseY = mousePos.y;
        int tileX = (mouseX / spriteWidth + mouseY / spriteHeight) / 2;
        int tileY = (mouseY / spriteHeight - mouseX / spriteWidth) / 2;

        // Check if mouse position is within the grid
        if (tileX >= 0 && tileX < numColumns && tileY >= 0 && tileY < numRows)
        {
            // Draw sprite at the mouse position (for visualization)
            DrawTextureEx(sprite, (Vector2){ tileX * spriteWidth, tileY * spriteHeight }, 0.0f, 1.0f, WHITE);
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




