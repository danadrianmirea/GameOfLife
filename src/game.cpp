#include <vector>
#include <utility>
#include <string>

#include "raylib.h"
#include "globals.h"
#include "game.h"

Game::Game(int width, int height, int offset, int cellSize) : grid(width, height, offset, cellSize), tempGrid(width, height, offset, cellSize)
{
    firstTimeGameStart = true;

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

    if (IsKeyPressed(KEY_ENTER) && !(IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
    {
        Randomize();
    }
}

void Game::UpdateUI()
{
    if (WindowShouldClose() || (IsKeyPressed(KEY_ESCAPE) && exitWindowRequested == false))
    {
        exitWindowRequested = true;
        isInExitMenu = true;
        return;
    }

    if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
    {
        windowWidth = GetScreenWidth();
        windowHeight = GetScreenHeight();

        ToggleBorderlessWindowed();
        // ToggleFullscreen();
        if (fullscreen)
        {
            fullscreen = false;
            SetWindowSize(windowWidth, windowHeight);
        }
        else
        {
            fullscreen = true;
            SetWindowSize(windowWidth, windowHeight);
        }
    }

    if (firstTimeGameStart && IsKeyPressed(KEY_SPACE))
    {
        firstTimeGameStart = false;
    }
    else if (gameOver && IsKeyPressed(KEY_SPACE))
    {
        Reset();
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

    if (exitWindowRequested == false && lostWindowFocus == false && gameOver == false && isFirstFrameAfterReset == false && IsKeyPressed(KEY_P))
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

    DrawScreenSpaceUI();
    EndDrawing();
}

void Game::DrawUI()
{
    // DrawRectangleRoundedLines({borderOffsetWidth, borderOffsetHeight, gameScreenWidth - borderOffsetWidth * 2, gameScreenHeight - borderOffsetHeight * 2}, 0.18f, 20, 2, yellow);
    DrawTextEx(font, "Conway's game of life", {200, 10}, 34, 2, yellow);
    DrawTextEx(font, "ESC to exit, P to pause, Enter to restart", {50, 50}, 34, 2, yellow);
}

void Game::DrawScreenSpaceUI()
{
    if (exitWindowRequested)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, BLACK);
        DrawText("Are you sure you want to exit? [Y/N]", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2, 40, yellow);
    }
    else if (firstTimeGameStart)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, BLACK);
        DrawText("Press SPACE to play", GetScreenWidth() / 2 - 200, GetScreenHeight() / 2, 40, yellow);
    }
    else if (paused)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, BLACK);
        DrawText("Game paused, press P to continue", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2, 40, yellow);
    }
    else if (lostWindowFocus)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, BLACK);
        DrawText("Game paused, focus window to continue", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2, 40, yellow);
    }
    else if (gameOver)
    {
        DrawRectangleRounded({(float)(GetScreenWidth() / 2 - 500), (float)(GetScreenHeight() / 2 - 40), 1000, 120}, 0.76f, 20, BLACK);
        DrawText("Game over, press SPACE to play again", GetScreenWidth() / 2 - 400, GetScreenHeight() / 2, 40, yellow);
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
