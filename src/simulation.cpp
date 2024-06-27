#include <vector>
#include <utility>
#include "raylib.h"
#include "simulation.h"

Simulation::Simulation(int width, int height, int offset, int cellSize) : grid(width, height-offset, offset, cellSize), tempGrid(width, height-offset, offset, cellSize), isPaused(false), elapsedTime(0.0f)
{
}

void Simulation::Update()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        if (isPaused)
        {
            isPaused = false;
        }
        else
        {
            isPaused = true;
        }
    }

    if(IsKeyPressed(KEY_ENTER))
    {
        Randomize();
    }

    if (isPaused)
    {
        return;
    }

/*
    elapsedTime += GetFrameTime();
    if (elapsedTime >= resetTime)
    {
        Randomize();
        elapsedTime = 0.0f;
    }
    */

    for (int row = 0; row < grid.GetRows(); row++)
    {

        for (int column = 0; column < grid.GetColumns(); column++)
        {
            int liveNeighbors = CountLiveNeighbors(row, column);
            int cellValue = grid.GetValue(row, column);

            if (cellValue)
            {
                if (liveNeighbors > 3 || liveNeighbors < 2)
                {
                    tempGrid.SetValue(row, column, 0);
                }
                else
                {
                    tempGrid.SetValue(row, column, 1);
                }
            }
            else
            {
                if (liveNeighbors == 3)
                {
                    tempGrid.SetValue(row, column, 1);
                }
                else
                {
                    tempGrid.SetValue(row, column, 0);
                }
            }
        }
    }

    grid = tempGrid;
}

void Simulation::Draw()
{
    grid.Draw();
}

void Simulation::Randomize()
{
    grid.Randomize();
}

void Simulation::SetCellValue(int row, int column, int value)
{
    grid.SetValue(row, column, value);
}

int Simulation::CountLiveNeighbors(int row, int column)
{
    if (grid.IsWithinBounds(row, column) == false)
    {
        return 0;
    }

    int numAlive = 0;

    std::vector<std::pair<int, int>> neighborOffsets =
        {
            {-1, 0},
            {1, 0},
            {0, -1},
            {0, 1},
            {-1, -1},
            {-1, 1},
            {1, -1},
            {1, 1}};

    for (auto &offset : neighborOffsets)
    {
        if (grid.GetValue(row + offset.first, column + offset.second))
            numAlive++;
    }

    return numAlive;
}
