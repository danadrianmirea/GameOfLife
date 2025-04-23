#pragma once

#include "grid.h"
#include <string>

class Game
{
public:
    Game(int width, int height, int offset, int cellSize);
    ~Game();
    void InitGame();
    void Reset();
    void Update(float dt);
    void HandleInput();
    void UpdateUI();

    void Draw();
    void DrawUI();
    void DrawScreenSpaceUI();
    std::string FormatWithLeadingZeroes(int number, int width);
    void Randomize();
    void SetCellValue(int row, int column, int value);
    int CountLiveNeighbors(int row, int column);

    static bool isMobile;

private:
    bool firstTimeGameStart;
    bool isFirstFrameAfterReset;
    bool isInExitMenu;
    bool paused;
    bool lostWindowFocus;
    bool gameOver;

    float screenScale;
    RenderTexture2D targetRenderTex;
    Font font;

    Grid grid;
    Grid tempGrid;

    int width;
    int height;
    int offset;
    int cellSize;
};