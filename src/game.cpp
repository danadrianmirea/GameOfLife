#include <vector>
#include <utility>
#include <string>

#include "raylib.h"
#include "globals.h"
#include "game.h"

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

bool Game::isMobile = false;

Game::Game(int width, int height, int offset, int cellSize) : grid(width, height, offset, cellSize), tempGrid(width, height, offset, cellSize)
{
    firstTimeGameStart = true;

#ifdef __EMSCRIPTEN__
    // Check if we're running on a mobile device
    isMobile = EM_ASM_INT({
        return /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent);
    });
#endif

    targetRenderTex = LoadRenderTexture(gameScreenWidth, gameScreenHeight);
    SetTextureFilter(targetRenderTex.texture, TEXTURE_FILTER_BILINEAR); // Texture scale filter to use

    font = LoadFontEx("Font/monogram.ttf", 64, 0, 0);

    this->width = width;
    this->height = height;
    this->offset = offset;
    this->cellSize = cellSize;

    InitGame();
}

Game::~Game()
{
    UnloadRenderTexture(targetRenderTex);
    UnloadFont(font);
}

void Game::InitGame()
{
    isFirstFrameAfterReset = true;
    isInExitMenu = false;
    paused = false;
    lostWindowFocus = false;
    gameOver = false;

    screenScale = MIN((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);
    grid = Grid(width, height - offset, offset, cellSize);
    tempGrid = Grid(width, height - offset, offset, cellSize);
}

void Game::Reset()
{
    InitGame();
}

void Game::Update(float dt)
{
    if (dt == 0)
    {
        return;
    }

    screenScale = MIN((float)GetScreenWidth() / gameScreenWidth, (float)GetScreenHeight() / gameScreenHeight);
    UpdateUI();

    bool running = (firstTimeGameStart == false && paused == false && lostWindowFocus == false && isInExitMenu == false && gameOver == false);

    if (running)
    {
        HandleInput();

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
}

void Game::HandleInput()
{
    if (isFirstFrameAfterReset)
    {
        isFirstFrameAfterReset = false;
        return;
    }

    if (isMobile) {
        if (IsGestureDetected(GESTURE_TAP)) {
            Randomize();
        }
    } else {
        if (IsKeyPressed(KEY_ENTER) && !(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        {
            Randomize();
        }
    }
}

void Game::UpdateUI()
{
#ifndef EMSCRIPTEN_BUILD
    if (WindowShouldClose() || (IsKeyPressed(KEY_ESCAPE) && exitWindowRequested == false))
    {
        exitWindowRequested = true;
        isInExitMenu = true;
        return;
    }

    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
    {
        if (fullscreen)
        {
            fullscreen = false;
            ToggleBorderlessWindowed();
        }
        else
        {
            fullscreen = true;
            ToggleBorderlessWindowed();
        }
    }
#endif

    if (isMobile) {
        if (IsGestureDetected(GESTURE_TAP)) {
            if (firstTimeGameStart) {
                firstTimeGameStart = false;
            } else if (gameOver) {
                Reset();
            } else if (paused) {
                paused = false;
            }
        }
    } else {
        if (firstTimeGameStart && IsKeyPressed(KEY_ENTER))
        {
            firstTimeGameStart = false;
        }
        else if (gameOver && IsKeyPressed(KEY_ENTER))
        {
            Reset();
        }
    }

    if (exitWindowRequested)
    {
        if (IsKeyPressed(KEY_Y))
        {
            exitWindow = true;
        }
        else if (IsKeyPressed(KEY_N) || IsKeyPressed(KEY_ESCAPE))
        {
            exitWindowRequested = false;
            isInExitMenu = false;
        }
    }

    if (IsWindowFocused() == false)
    {
        lostWindowFocus = true;
    }
    else
    {
        lostWindowFocus = false;
    }

#ifndef EMSCRIPTEN_BUILD
    if (exitWindowRequested == false && lostWindowFocus == false && gameOver == false && isFirstFrameAfterReset == false && IsKeyPressed(KEY_P))
#else
    if (exitWindowRequested == false && lostWindowFocus == false && gameOver == false && isFirstFrameAfterReset == false && (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ESCAPE)))
#endif
    {
        if (paused)
        {
            paused = false;
        }
        else
        {
            paused = true;
        }
    }
}

void Game::Draw()
{
    // render everything to a texture
    BeginTextureMode(targetRenderTex);
    ClearBackground(black);

    grid.Draw();
    DrawUI();

    EndTextureMode();

    // render the scaled frame texture to the screen
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(targetRenderTex.texture, (Rectangle){0.0f, 0.0f, (float)targetRenderTex.texture.width, (float)-targetRenderTex.texture.height},
                   (Rectangle){(GetScreenWidth() - ((float)gameScreenWidth * screenScale)) * 0.5f, (GetScreenHeight() - ((float)gameScreenHeight * screenScale)) * 0.5f, (float)gameScreenWidth * screenScale, (float)gameScreenHeight * screenScale},
                   (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
}

void Game::DrawUI()
{
    float screenX = 0.0f;
    float screenY = 0.0f;

    // DrawRectangleRoundedLines({borderOffsetWidth, borderOffsetHeight, gameScreenWidth - borderOffsetWidth * 2, gameScreenHeight - borderOffsetHeight * 2}, 0.18f, 20, 2, yellow);
    DrawTextEx(font, "Conway's game of life", {200, 10}, 34, 2, yellow);
    if (isMobile) {
        DrawTextEx(font, "Tap screen to start/reset", {160, 50}, 34, 2, yellow);
    } else {
        DrawTextEx(font, "ESC to exit, P to pause, Enter to restart", {50, 50}, 34, 2, yellow);
    }

    if (exitWindowRequested)
    {
        DrawRectangleRounded({screenX + (float)(gameScreenWidth / 2 - 250), screenY + (float)(gameScreenHeight / 2 - 20), 500, 60}, 0.76f, 20, BLACK);
        DrawText("Are you sure you want to exit? [Y/N]", screenX + (gameScreenWidth / 2 - 200), screenY + gameScreenHeight / 2, 20, yellow);
    }
    else if (firstTimeGameStart)
    {
        DrawRectangleRounded({screenX + (float)(gameScreenWidth / 2 - 250), screenY + (float)(gameScreenHeight / 2 - 20), 500, 60}, 0.76f, 20, BLACK);
        if (isMobile) {
            DrawText("Tap to play", screenX + (gameScreenWidth / 2 - 60), screenY + gameScreenHeight / 2, 20, yellow);
        } else {
            DrawText("Press Enter to play", screenX + (gameScreenWidth / 2 - 100), screenY + gameScreenHeight / 2, 20, yellow);
        }
    }
    else if (paused)
    {
        DrawRectangleRounded({screenX + (float)(gameScreenWidth / 2 - 250), screenY + (float)(gameScreenHeight / 2 - 20), 500, 60}, 0.76f, 20, BLACK);
#ifndef EMSCRIPTEN_BUILD
        DrawText("Game paused, press P to continue", screenX + (gameScreenWidth / 2 - 200), screenY + gameScreenHeight / 2, 20, yellow);
#else
        if (isMobile) {
            DrawText("Game paused, tap to continue", screenX + (gameScreenWidth / 2 - 200), screenY + gameScreenHeight / 2, 20, yellow);
        } else {
            DrawText("Game paused, press P or ESC to continue", screenX + (gameScreenWidth / 2 - 200), screenY + gameScreenHeight / 2, 20, yellow);
        }
#endif
    }
    else if (lostWindowFocus)
    {
        DrawRectangleRounded({screenX + (float)(gameScreenWidth / 2 - 250), screenY + (float)(gameScreenHeight / 2 - 20), 500, 60}, 0.76f, 20, BLACK);
        DrawText("Game paused, focus window to continue", screenX + (gameScreenWidth / 2 - 200), screenY + gameScreenHeight / 2, 20, yellow);
    }
    else if (gameOver)
    {
        DrawRectangleRounded({screenX + (float)(gameScreenWidth / 2 - 250), screenY + (float)(gameScreenHeight / 2 - 20), 500, 60}, 0.76f, 20, BLACK);
        if (isMobile) {
            DrawText("Game over, tap to play again", screenX + (gameScreenWidth / 2 - 200), screenY + gameScreenHeight / 2, 20, yellow);
        } else {
            DrawText("Game over, press Enter to play again", screenX + (gameScreenWidth / 2 - 200), screenY + gameScreenHeight / 2, 20, yellow);
        }
    }
}

std::string Game::FormatWithLeadingZeroes(int number, int width)
{
    std::string numberText = std::to_string(number);
    int leadingZeros = width - numberText.length();
    numberText = std::string(leadingZeros, '0') + numberText;
    return numberText;
}

void Game::Randomize()
{
    grid.Randomize();
}

void Game::SetCellValue(int row, int column, int value)
{
    grid.SetValue(row, column, value);
}

int Game::CountLiveNeighbors(int row, int column)
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
