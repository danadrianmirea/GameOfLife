#pragma once

#include <vector>

class Grid
{
public:
    Grid(int width, int height, int offset, int cellSize);
    void Randomize();
    void Draw();
    void SetValue(int row, int column, int value);
    int GetValue(int row, int column);
    bool IsWithinBounds(int row, int column);

    int GetRows() { return rows; }
    int GetColumns() { return columns; }
    int GetCellSize() { return cellSize; }

private:
    int rows;
    int columns;
    int offset;
    int cellSize;
    int borderSize = 1;
    std::vector<std::vector<int>> cells;

    Color aliveColor{0, 255, 0, 255};
    Color deadColor{0, 0, 0, 255};
};