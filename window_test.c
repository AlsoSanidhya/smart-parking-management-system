#include "raylib.h"

int main(void)
{
    InitWindow(800, 500, "Smart Parking - Window Test");
    SetTargetFPS(60);
    int occupied = 1;
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE))
{
    occupied = !occupied;
}
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Smart Parking Management System", 40, 40, 30, DARKBLUE);
        DrawRectangle(50, 120, 90, 140, occupied ? RED : GREEN);
        DrawText("P1", 80, 180, 30, BLACK);
        DrawRectangle(170, 120, 90, 140, GREEN);
        DrawText("P2", 200, 180, 30, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}