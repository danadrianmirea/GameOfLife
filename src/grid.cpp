#include "raylib.h"
#include "grid.h"

Grid::Grid(int width, int height, int offset, int cellSize) : rows(height / cellSize),
                                                              columns(width / cellSize),
                                                              offset(offset),
                                                              cellSize(cellSize),
                                                              cells(rows, std::vector<int>(columns, 0))
{
}

void Grid::Randomize()
{
    for (int row = 0; row < cells.size(); row++)
    {
        for (int column = 0; column < cells[row].size(); column++)
        {
            cells[row][column] = GetRandomValue(0, 1);
        }
    }
}

void Grid::Draw()
{
    for (int row = 0; row < cells.size(); row++)
    {
        for (int column = 0; column < cells[row].size(); column++)
        {
            Color color = cells[row][column] ? aliveColor : deadColor;
            DrawRectangle(column * cellSize, row * cellSize + offset, cellSize - borderSize, cellSize - borderSize, color);
        }
    }
}

void Grid::SetValue(int row, int column, int value)
{
    if(IsWithinBounds(row, column))
    {
        cells[row][column] = value;
    }
}

int Grid::GetValue(int row, int column)
{
    if(IsWithinBounds(row, column))
    {
        return cells[row][column];
    }
    else
    {
        return 0;
    }
}

bool Grid::IsWithinBounds(int row, int column)
{
    if (row > 0 && row < rows-1 && column > 0 && column < columns-1)
    {
        return true;
    }
    return false;
}
