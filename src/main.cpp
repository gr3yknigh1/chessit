#include <raylib.h>

int main(void)
{
    InitWindow(900, 600, "ChessIT");

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
