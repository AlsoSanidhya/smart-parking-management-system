#include <stdio.h>
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


void DrawLabel(Font font, const char *text,int x, int y, float size, Color color)
{
    DrawTextEx(font, text,(Vector2){x, y},size, 1.0f, color);
}

void mergeSortSlots(int slots[], int temp[], const int rates[],
                    int left, int right)
{
    if (left >= right)
    {
        return;
    }

    int middle = left + (right - left) / 2;

    mergeSortSlots(slots, temp, rates, left, middle);
    mergeSortSlots(slots, temp, rates, middle + 1, right);

    int first = left;
    int second = middle + 1;
    int next = left;

    while (first <= middle && second <= right)
    {
        if (rates[slots[first]] <= rates[slots[second]])
        {
            temp[next] = slots[first];
            first++;
        }
        else
        {
            temp[next] = slots[second];
            second++;
        }
        next++;
    }

    while (first <= middle)
    {
        temp[next] = slots[first];
        first++;
        next++;
    }

    while (second <= right)
    {
        temp[next] = slots[second];
        second++;
        next++;
    }

    for (int i = left; i <= right; i++)
    {
        slots[i] = temp[i];
    }
}

#define NODES 7
#define INF 1000000

void calculateDistances(int distance[NODES])
{
    // Node order: Entrance, Junction, P1, P2, P3, P4, P5.
    // A zero means there is no direct lane between those nodes.
    int lanes[NODES][NODES] = {
        { 0, 10,  0, 35,  0,  0,  0}, // Entrance
        {10,  0, 20, 12, 35, 18, 30}, // Junction
        { 0, 20,  0,  0,  0,  0,  0}, // P1
        {35, 12,  0,  0,  0,  0,  0}, // P2
        { 0, 35,  0,  0,  0,  0,  0}, // P3
        { 0, 18,  0,  0,  0,  0,  0}, // P4
        { 0, 30,  0,  0,  0,  0,  0}  // P5
    };

    int visited[NODES] = {0};

    for (int i = 0; i < NODES; i++)
    {
        distance[i] = INF;
    }
    distance[0] = 0; // Distance from the entrance to itself.

    for (int step = 0; step < NODES; step++)
    {
        int closest = -1;
        int best = INF;

        for (int i = 0; i < NODES; i++)
        {
            if (!visited[i] && distance[i] < best)
            {
                best = distance[i];
                closest = i;
            }
        }

        if (closest == -1)
        {
            break;
        }

        visited[closest] = 1;

        for (int next = 0; next < NODES; next++)
        {
            if (!visited[next] && lanes[closest][next] > 0)
            {
                int newDistance = distance[closest] + lanes[closest][next];

                if (newDistance < distance[next])
                {
                    distance[next] = newDistance;
                }
            }
        }
    }
}
int main(void)
{
    InitWindow(1000, 650, "Smart Parking Management");
    SetTargetFPS(60);
    Font uiFont = LoadFontEx("C:/Windows/Fonts/segoeui.ttf",40,NULL,0);
    if (uiFont.texture.id == 0)
    {
        printf("Could not load Segoe UI font.\n");
        CloseWindow();
        return 1;
    }
    
    int occupied[5] = {0}; // Array to track occupied parking spaces
    char parkedPlates[5][16] = {0};
    int slotsChecked = 0; 
    int lastAssigned = -1;
    char vehicleNumber[16] = "";
    int vehicleLength = 0;
    int duplicatePlate = 0;
    Rectangle parkButton = {790, 300, 170, 50};
    Rectangle findButton = {790, 370, 170, 50};
    Rectangle cheapestButton = {790, 440, 170, 50};
    Rectangle nearestButton = {790, 510, 170, 50};
    int foundSlot = -2;
    int hourlyRate[5] = {40, 25, 30, 20, 35};
    int distance[NODES];calculateDistances(distance);
    FILE *savedParking = fopen("parking.txt", "r");

    if (savedParking != NULL)
    {
        int slot;
        char plate[16];

        while (fscanf(savedParking, "%d %15s", &slot, plate) == 2)
        {
            if (slot >= 0 && slot < 5)
            {
            strcpy(parkedPlates[slot], plate);
            occupied[slot] = 1;
            }
        }

        fclose(savedParking);
    }

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
                Rectangle slot = {50 + i * 135, 120, 100, 150};
                if (CheckCollisionPointRec(mouse, slot) && occupied[i])
                {
                    occupied[i] = 0;
                    parkedPlates[i][0] = '\0';
                }
            }
        }
        int clicked = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        int chooseCheapest = clicked && CheckCollisionPointRec(GetMousePosition(), cheapestButton);

        if (vehicleLength > 0 && clicked && (chooseCheapest || CheckCollisionPointRec(GetMousePosition(), parkButton)))
        {
            slotsChecked = 0;
            lastAssigned = -1;
            duplicatePlate = findVehicle(parkedPlates, vehicleNumber) >= 0;

            for (int i = 0; i < 5 && !duplicatePlate; i++)
            {
                slotsChecked++;

                if (occupied[i] == 0 && (lastAssigned == -1 || hourlyRate[i] < hourlyRate[lastAssigned]))
                {
                    lastAssigned = i;

                    if (!chooseCheapest)
                    {
                        break;
                    }
                }
        
            }

    if (lastAssigned >= 0)
    {
        occupied[lastAssigned] = 1;
        strcpy(parkedPlates[lastAssigned], vehicleNumber);
        vehicleNumber[0] = '\0';
        vehicleLength = 0;
    }
}
        if (vehicleLength > 0 && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), findButton))
        {
            foundSlot = findVehicle(parkedPlates, vehicleNumber);
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawLabel(uiFont,"Smart Parking Management System",40, 40, 32, DARKBLUE); 
        DrawLabel(uiFont,"Enter a vehicle number to book a slot. Click an occupied slot to release it.",40, 85, 18, DARKGRAY);


        for (int i = 0; i < 5; i++) // Draw parking spaces
        {
            int x = 50 + i * 135;

            
        Color freeColor = {190, 235, 205, 255};
        Color occupiedColor = {245, 195, 195, 255};

        DrawRectangleRounded((Rectangle){x, 120, 100, 150},0.12f,8,occupied[i] ? occupiedColor : freeColor);


            DrawLabel(uiFont, TextFormat("P%d", i + 1),x + 30, 180, 30, BLACK);
            DrawLabel(uiFont, parkedPlates[i], x + 5, 225, 15, BLACK);
            DrawLabel(uiFont, TextFormat("Rs %d/hr", hourlyRate[i]),x + 8, 145, 16, BLACK);     
        }

       
        int available = 0;
        int freeSlots[5];

        for (int i = 0; i < 5; i++)
        {
            if (occupied[i] == 0)
            {
                freeSlots[available] = i;
                available++;
            }
        }

        int temp[5];mergeSortSlots(freeSlots, temp, hourlyRate, 0, available - 1);  

        DrawLabel(uiFont,TextFormat("Available: %d / 5", available),50, 340, 25, DARKBLUE);

        if (available == 0)
        {
        DrawLabel(uiFont, "Parking Full",50, 380, 23, RED);
        }

        DrawLabel(uiFont,TextFormat("Slots checked: %d", slotsChecked),50, 420, 20, DARKGRAY);

        if (duplicatePlate)
        {
        DrawLabel(uiFont, "Vehicle already parked",50, 460, 20, RED);
        }
        else if (lastAssigned >= 0)
        {
            DrawLabel(uiFont,TextFormat("Last assignment: P%d (%d m)",lastAssigned + 1, distance[lastAssigned + 2]),50, 460, 20, DARKGRAY);
        }
        else if (slotsChecked > 0)
        {
            DrawLabel(uiFont, "No free slot found",50, 460, 20, RED);
        }

        DrawRectangleRounded(parkButton, 0.20f, 8, DARKBLUE);
        DrawLabel(uiFont, "Park Car", 825, 310, 22, WHITE);
        DrawLabel(uiFont, "Vehicle Number:", 50, 290, 22, DARKBLUE);
        Rectangle inputBox = {245, 282, 250, 40};
        DrawRectangleRounded(inputBox, 0.15f, 8, LIGHTGRAY);
        DrawLabel(uiFont, vehicleNumber, 260, 290, 22, BLACK);
        DrawRectangleRounded(findButton, 0.20f, 8, DARKBLUE);
        DrawLabel(uiFont, "Find Car", 825, 380, 22, WHITE);

       if (foundSlot >= 0)
        {
            DrawLabel(uiFont,TextFormat("Last lookup: P%d", foundSlot + 1),50, 505, 20, DARKBLUE);
        }
        else if (foundSlot == -1)
        {
            DrawLabel(uiFont, "Last lookup: Vehicle not found",50, 505, 20, RED);
        }
            DrawRectangleRounded(cheapestButton, 0.20f, 8, DARKBLUE);
            DrawLabel(uiFont, "Park Cheapest", 800, 450, 19, WHITE);
            DrawRectangleRounded(nearestButton, 0.20f, 8, DARKBLUE);
            DrawLabel(uiFont, "Park Nearest", 815, 520, 20, WHITE);
            DrawLabel(uiFont, "Compare Prices - Free Slots", 50, 555, 22, DARKBLUE);

        if (available == 0)
        {
            DrawLabel(uiFont, "No free slots to compare.", 50, 590, 18, DARKGRAY);
        }

        for (int i = 0; i < available; i++)
        {
            int slot = freeSlots[i];

            DrawLabel(uiFont,TextFormat("P%d: Rs %d/hr", slot + 1, hourlyRate[slot]),50 + i * 145, 590, 20, BLACK);
        }   
            EndDrawing();
    }
    FILE *parkingOutput = fopen("parking.txt", "w");

    if (parkingOutput != NULL)
    {
        for (int i = 0; i < 5; i++)
        {
            if (occupied[i])
            {
                fprintf(parkingOutput, "%d %s\n", i, parkedPlates[i]);
            }
        }

    if (fclose(parkingOutput) != 0)
    {
        puts("Could not finish saving parking.txt.");
    }
    }
    else
    {
        puts("Could not open parking.txt for saving.");
    }
    UnloadFont(uiFont);
    CloseWindow();
    return 0;
}