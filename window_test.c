#include "raylib.h"
#include <ctype.h>
#include <string.h>

int findVehicle(char parkedPlates[5][16], const char *plate)
{
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(parkedPlates[i], plate) == 0)
        {
            return i;
        }
    }

    return -1;
}

int main(void)
{
    InitWindow(800, 500, "Smart Parking - Window Test");
    SetTargetFPS(60);

    int occupied[5] = {0}; // Array to track occupied parking spaces
    char parkedPlates[5][16] = {0};
    int slotsChecked = 0; 
    int lastAssigned = -1;
    Rectangle parkButton = {600, 320, 150, 45};
    char vehicleNumber[16] = "";
    int vehicleLength = 0;
    int duplicatePlate = 0;
    Rectangle findButton = {600, 380, 150, 45};
    int foundSlot = -2;
    int hourlyRate[5] = {40, 25, 30, 20, 35};

    while (!WindowShouldClose())
    {
        int key = GetCharPressed();

        if (key >= 32 && key <= 126 && vehicleLength < 10)
        {
            key = toupper(key);

            if ((key >= 'A' && key <= 'Z') || (key >= '0' && key <= '9'))
            {
                vehicleNumber[vehicleLength] = (char)key;
                vehicleLength++;
                vehicleNumber[vehicleLength] = '\0';
            }
        }

        if (IsKeyPressed(KEY_BACKSPACE) && vehicleLength > 0)
        {
        vehicleLength--;
        vehicleNumber[vehicleLength] = '\0';
        }

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) // Check for mouse click to toggle parking space occupancy
        {
            Vector2 mouse = GetMousePosition();
            for (int i = 0; i < 5; i++)
            {
                Rectangle slot = {50 + i * 120, 120, 90, 140};
                if (CheckCollisionPointRec(mouse, slot))
                {
                    occupied[i] = 0;
                    parkedPlates[i][0] = '\0';
                }
            }
        }
        if (vehicleLength > 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), parkButton))
        {
            slotsChecked = 0;
            lastAssigned = -1;

            duplicatePlate = findVehicle(parkedPlates, vehicleNumber) >= 0;

            for (int i = 0; i < 5 && !duplicatePlate; i++)
            {
                slotsChecked++;
                if (occupied[i] == 0)
                {
                    occupied[i] = 1;
                    strcpy(parkedPlates[i], vehicleNumber);
                    vehicleNumber[0] = '\0';
                    vehicleLength = 0;
                    lastAssigned = i;
                    break;
                }
            }
        }
        if (vehicleLength > 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), findButton))
        {
            foundSlot = findVehicle(parkedPlates, vehicleNumber);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Smart Parking Management System", 40, 40, 30, DARKBLUE); 
        DrawText("Type a plate and click Park Car. Click a parked car to check out.", 40, 85, 18, DARKGRAY);


        for (int i = 0; i < 5; i++) // Draw parking spaces
        {
            int x = 50 + i * 120;

            DrawRectangle(x, 120, 90, 140,
                        occupied[i] ? RED : GREEN);

            DrawText(TextFormat("P%d", i + 1),
                     x + 30, 180, 30, BLACK);
            DrawText(parkedPlates[i], x + 5, 225, 12, BLACK);    
            DrawText(TextFormat("Rs %d/hr", hourlyRate[i]),
            x + 8, 145, 14, BLACK);     
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
        if (duplicatePlate)
        {
            DrawText("Vehicle already parked", 50, 435, 20, RED);
        }
        else if (lastAssigned >= 0)
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
        DrawText("Vehicle number:", 50, 280, 20, DARKBLUE);
        DrawText(vehicleNumber, 240, 280, 20, BLACK);

        DrawRectangleRec(findButton, DARKBLUE);
        DrawText("Find Car", 620, 392, 20, WHITE);

        if (foundSlot >= 0)
        {
            DrawText(TextFormat("Last lookup: P%d", foundSlot + 1),
             50, 465, 20, DARKBLUE);
        }
        else if (foundSlot == -1)
        {
            DrawText("Last lookup: Vehicle not found", 50, 465, 20, RED);
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}