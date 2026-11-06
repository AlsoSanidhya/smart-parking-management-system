#include "raylib.h"

int main(void)
{
    InitWindow(800, 500, "Smart Parking - Window Test");
    SetTargetFPS(60);

    int occupied[5] = {0}; // Array to track occupied parking spaces
    int slotsChecked = 0; 
    int lastAssigned = -1;
    Rectangle parkButton = {600, 320, 150, 45};

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
            slotsChecked = 0;
            lastAssigned = -1;
            for (int i = 0; i < 5; i++)
            {
                slotsChecked++;
                if (occupied[i] == 0)
                {
                    occupied[i] = 1;
                    lastAssigned = i;
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
        DrawText(TextFormat("Slots checked on last search: %d", slotsChecked),
         50, 400, 20, DARKGRAY);
        if (lastAssigned >= 0)
        {
            DrawText(TextFormat("Last assignment: P%d", lastAssigned + 1),
                     50, 435, 20, DARKGRAY);
        }
        else if (slotsChecked > 0)
        {
            DrawText("No free slot found", 50, 435, 20, RED);
        }
        DrawRectangleRec(parkButton, DARKBLUE);
        DrawText("Park Car", 620, 332, 20, WHITE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}