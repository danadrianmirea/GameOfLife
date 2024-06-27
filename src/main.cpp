
#include "raylib.h"
#include "simulation.h"

#include <iostream>

Color grey = {29, 29, 29, 255};
int screenWidth;
int screenHeight;
int offset;
int numCells;
int fps;
float elapsedTime = 0.0f;
const float resetTime = 20.0f;

/*
std::vector<std::vector<int>> vec =
    {
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
*/

int main()
{

    /*
    InitWindow(0, 0, "Game of life");
    screenWidth = GetMonitorWidth(GetCurrentMonitor());
    screenHeight = GetMonitorHeight(GetCurrentMonitor());
    SetWindowSize(screenWidth, screenHeight);
    */

    screenWidth = 1280;
    screenHeight = 720;
    offset = 70;
    numCells = 256;
    fps = 144;

    InitWindow(screenWidth, screenHeight, "Game of life");

    SetTargetFPS(fps);

    Simulation simulation(screenWidth, screenHeight, offset, screenWidth / (numCells + 2));

    simulation.Randomize();

    while (!WindowShouldClose())
    {
        simulation.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        simulation.Draw();

        DrawText("Conway's game of life. ESC to exit, SPACE to pause, ENTER to randomize", 100, 20, 30, GREEN);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}