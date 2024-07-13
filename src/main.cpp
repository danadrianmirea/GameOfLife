
#include "raylib.h"
#include "globals.h"
#include "game.h"
#include <iostream>

int main()
{
    InitWindow(gameScreenWidth, gameScreenHeight, "Game of life");
    windowWidth = GetScreenWidth();
    windowHeight = GetScreenHeight();
    SetExitKey(KEY_NULL);
    SetWindowSize(windowWidth, windowHeight);
    SetWindowPosition(50, 50);
    SetTargetFPS(144);

    const int offset = 110;
    const int numCells = 150;

    Game game(gameScreenWidth, gameScreenHeight, offset, gameScreenWidth / (numCells + 2));
    game.Randomize();
    ToggleBorderlessWindowed();
    float dt = 0.0f;

    while (!exitWindow)
    {
        dt = GetFrameTime();
        game.Update(dt);
        game.Draw();
    }

    CloseWindow();
    return 0;
}