#include "raylib-cpp.hpp"

int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;

    raylib::Window window(screenWidth, screenHeight, "Raylib-cpp CMake Projesi");
    SetTargetFPS(60);

    raylib::Vector2 circlePos(screenWidth / 2.0f, screenHeight / 2.0f);

    while (!window.ShouldClose()) {
        // Girdi
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) circlePos.x += 4.0f;
        if (IsKeyDown(KEY_LEFT)  || IsKeyDown(KEY_A)) circlePos.x -= 4.0f;
        if (IsKeyDown(KEY_UP)    || IsKeyDown(KEY_W)) circlePos.y -= 4.0f;
        if (IsKeyDown(KEY_DOWN)  || IsKeyDown(KEY_S)) circlePos.y += 4.0f;

        // Çizim
        window.BeginDrawing();
        window.ClearBackground(RAYWHITE);

        DrawText("Raylib-cpp sorunsuz calisiyor!", 20, 20, 20, DARKGRAY);


        DrawCircleV(circlePos, 25.0f, MAROON);

        window.EndDrawing();
    }

    return 0;
}