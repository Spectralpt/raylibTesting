#include "raylib.h"

#define NUM_SPACES 30

int main()
{
    // Inicializa a janela
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Jogo do Ganso");

    // Carrega a textura do tabuleiro
    Texture2D board = LoadTexture("tabuleiro.png");

    // Define as posições das casas do jogo do ganso
    Vector2 spaces[NUM_SPACES] = {
        (Vector2){18, 415}, (Vector2){90, 400}, (Vector2){180, 380}, (Vector2){265, 360},
        (Vector2){350, 345}, (Vector2){438, 325}, (Vector2){523, 305}, (Vector2){608, 285},
        (Vector2){690, 265}, (Vector2){778, 250}, (Vector2){690, 220}, (Vector2){608, 200},
        (Vector2){523, 180}, (Vector2){438, 160}, (Vector2){350, 140}, (Vector2){265, 125},
        (Vector2){180, 105}, (Vector2){90, 85}, (Vector2){18, 70}, (Vector2){95, 40},
        (Vector2){180, 20}, (Vector2){265, 0}, (Vector2){350, -15}, (Vector2){438, -35},
        (Vector2){523, -55}, (Vector2){608, -75}, (Vector2){690, -95}, (Vector2){778, -110}
    };

    // Define a posição inicial do jogador
    int playerSpace = 0;

    // Cria o loop principal do jogo
    while (!WindowShouldClose())
    {
        // Atualiza a posição do jogador
        if (IsKeyPressed(KEY_SPACE))
        {
            playerSpace++;
        }

        // Verifica se o jogador chegou ao final
        if (playerSpace >= NUM_SPACES - 1)
        {
            playerSpace = NUM_SPACES - 1;
        }

        // Desenha o tabuleiro
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawTexture(board, 0, 0, WHITE);

        // Desenha o jogador
        DrawCircleV(spaces[playerSpace], 20, BLUE);

        EndDrawing();
    }

    // Encerra a janela
    UnloadTexture(board);
    CloseWindow();

    return 0;
}

