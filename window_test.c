#include "raylib.h"

int main(void)
{
    InitWindow(800, 500, "Smart Parking - Window Test");
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Smart Parking Management System", 40, 40, 30, DARKBLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}