#include <cstdint>
#include <iostream>
#include <raylib.h>
#include <raymath.h>

const char *WINDOW_TITLE = "ChessIT";
const Vector2 WINDOW_SIZE = {512, 512};
const float WINDOW_RATIO = WINDOW_SIZE.x / WINDOW_SIZE.y;

enum StretchMode
{
    Stretch,
    ByWidth,
    ByHeight,
};

Rectangle GetRenderTextureRectFromRatio(StretchMode mode, float ratio)
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    if (mode == StretchMode::ByWidth)
    {
        return Rectangle{0, 0, static_cast<float>(screenWidth),
                         static_cast<float>(screenWidth * ratio)};
    }
    else if (mode == StretchMode::ByHeight)
    {
        return Rectangle{0, 0, static_cast<float>(screenHeight * ratio),
                         static_cast<float>(screenHeight)};
    }

    return Rectangle{0, 0, static_cast<float>(screenWidth),
                     static_cast<float>(screenHeight)};
}

const Vector2 CELL_SIZE_IN_PIXELS = {22, 22};
const Vector2 BOARD_SIZE_IN_CELLS = {8, 8};
const Vector4 BOARD_PADDING_IN_PIXELS = {2, 2, 2, 2};
const Vector2 GRID_SIZE_IN_PIXELS =
    Vector2Multiply(CELL_SIZE_IN_PIXELS, BOARD_SIZE_IN_CELLS);
const Vector2 BOARD_SIZE_IN_PIXELS = {
    GRID_SIZE_IN_PIXELS.x +
        (BOARD_PADDING_IN_PIXELS.x + BOARD_PADDING_IN_PIXELS.z),
    GRID_SIZE_IN_PIXELS.y +
        (BOARD_PADDING_IN_PIXELS.y + BOARD_PADDING_IN_PIXELS.w)};

struct ColorTheme
{
    Color clearColor;
    Color background;
    Color whiteCell;
    Color blackCell;
    Color boardPadding;
};

const ColorTheme DEFAULT_COLORTHEME = {
    .clearColor = { 39, 39, 39, 255 },
    .background = { 39, 39, 39, 255 },
    .whiteCell = WHITE,
    .blackCell = { 39, 39, 39, 255 },
    .boardPadding = WHITE,
};

struct Board
{
    Vector2 position;
    Vector2 sizeInCells;
    Vector2 cellsSizeInPixels;
    Vector4 padding;
};

void DrawBoard(const Board &board, const ColorTheme &colors)
{
    DrawRectangle(board.position.x, board.position.y,
                  BOARD_SIZE_IN_PIXELS.x, BOARD_SIZE_IN_PIXELS.y, colors.boardPadding);
    bool drawWhite = true;
    for (uint64_t i = 0; i < board.sizeInCells.y; ++i)
    {
        int posY =
            board.cellsSizeInPixels.y * i + board.position.y + board.padding.y;

        for (uint64_t j = 0; j < board.sizeInCells.x; ++j)
        {
            int posX = board.cellsSizeInPixels.x * j + board.position.x +
                       board.padding.x;

            DrawRectangle(posX, posY, CELL_SIZE_IN_PIXELS.x,
                          CELL_SIZE_IN_PIXELS.y,
                          drawWhite ? colors.whiteCell : colors.blackCell);
            drawWhite = !drawWhite;
        }
        drawWhite = !drawWhite;
    }
}

Vector2 Vector2CenterPositionTo(Vector2 originSize, Vector2 size)
{
    Vector2 originHalf = Vector2Divide(originSize, {2, 2});
    Vector2 sizeHalf = Vector2Divide(size, {2, 2});
    return Vector2Subtract(originHalf, sizeHalf);
}

int main(void)
{
    InitWindow(WINDOW_SIZE.x, WINDOW_SIZE.y, WINDOW_TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetTraceLogLevel(LOG_TRACE);

    TraceLog(LOG_DEBUG, "Board size: %.0fx%.0f px", BOARD_SIZE_IN_PIXELS.x,
             BOARD_SIZE_IN_PIXELS.y);

    SetWindowMinSize(BOARD_SIZE_IN_PIXELS.x, BOARD_SIZE_IN_PIXELS.y);

    RenderTexture2D renderTexture =
        LoadRenderTexture(WINDOW_SIZE.x, WINDOW_SIZE.y);
    ColorTheme colorTheme = DEFAULT_COLORTHEME;

    Board board = {
        .position = Vector2CenterPositionTo(WINDOW_SIZE, BOARD_SIZE_IN_PIXELS),
        .sizeInCells = BOARD_SIZE_IN_CELLS,
        .cellsSizeInPixels = CELL_SIZE_IN_PIXELS,
        .padding = BOARD_PADDING_IN_PIXELS,
    };

    while (!WindowShouldClose())
    {
        BeginTextureMode(renderTexture);
        ClearBackground(colorTheme.clearColor);
        DrawBoard(board, colorTheme);
        EndTextureMode();

        BeginDrawing();
        DrawTexturePro(
            renderTexture.texture,
            Rectangle{0, 0, static_cast<float>(renderTexture.texture.width),
                      static_cast<float>(-renderTexture.texture.height)},
            GetRenderTextureRectFromRatio(StretchMode::ByHeight, WINDOW_RATIO),
            Vector2{0, 0}, 0, WHITE);
        EndDrawing();
    }

    UnloadRenderTexture(renderTexture);
    CloseWindow();

    return 0;
}
