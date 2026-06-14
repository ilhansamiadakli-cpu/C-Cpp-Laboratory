#include <raylib.h>

int main() {
    InitWindow(800, 450, "Raylib - macOS Test");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Raylib calisiyor!", 300, 200, 20, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}