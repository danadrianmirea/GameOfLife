#pragma once

#include "grid.h"

class Simulation
{
public:
    Simulation(int width, int height, int offset, int cellSize);
    void Update();
    void Draw();
    void Randomize();
    void SetCellValue(int row, int column, int value);
    int CountLiveNeighbors(int row, int column);
    bool GetIsPaused() { return isPaused; }
    void SetIsPaused(bool paused) { isPaused = paused; }

private:

    float elapsedTime;
    const float resetTime = 10.0f;
    bool isPaused;
    Grid grid;
    Grid tempGrid;
};