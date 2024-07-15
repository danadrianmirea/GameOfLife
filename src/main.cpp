
#include "raylib.h"
#include "globals.h"
#include "game.h"
#include <iostream>

int main()
{
    InitWindow(gameScreenWidth, gameScreenHeight, "Game of life");
    SetExitKey(KEY_NULL);
    SetTargetFPS(144);
    
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