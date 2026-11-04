#include "raylib.h"

int main(void)
{
    InitWindow(800, 500, "Smart Parking - Window Test");
    SetTargetFPS(60);

    int occupied[5] = {0}; // Array to track occupied parking spaces

    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_ONE))
        {
            occupied[0] = !occupied[0];
        }
        if (IsKeyPressed(KEY_TWO))
        {
            occupied[1] = !occupied[1];
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // Check for mouse click to toggle parking space occupancy
        {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 5; i++)
            {
                Rectangle slot = {50 + i * 120, 120, 90, 140};
                if (CheckCollisionPointRec(mouse, slot))
                {
                    occupied[i] = !occupied[i];
                }
            }
        }
        if (IsKeyPressed(KEY_A))
        {
            for (int i = 0; i < 5; i++)
            {
                if (occupied[i] == 0)
                {
                    occupied[i] = 1;
                    break;
                }
            }
        }
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Smart Parking Management System", 40, 40, 30, DARKBLUE); 

        for (int i = 0; i < 5; i++) // Draw parking spaces
        {
            int x = 50 + i * 120;

            DrawRectangle(x, 120, 90, 140,
                        occupied[i] ? RED : GREEN);

            DrawText(TextFormat("P%d", i + 1),
                     x + 30, 180, 30, BLACK);
        }

        int available = 0; // Count available parking spaces
        for (int i = 0; i < 5; i++)
        {
            if (occupied[i] == 0)
            {
                available++;
            }
        }
        DrawText(TextFormat("Available: %d / 5", available),
                 50, 320, 25, DARKBLUE);  // Display available parking spaces
        if (available == 0)
        {
            DrawText("Parking Full", 50, 360, 25, RED); // Display "Parking Full" message
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}