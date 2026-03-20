#include "raylib.h"

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib hello world");

    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close or ESC key
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawText("Hello, world!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();

    return 0;
}