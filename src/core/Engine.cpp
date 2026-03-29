#include "core/Engine.h"
#include "raylib.h"

void Engine::Init(int width, int height, const std::string& title, int targetFps) {
    InitWindow(width, height, title.c_str());
    SetTargetFPS(targetFps);
}

void Engine::Run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        sceneManager.ProcessFrame(dt);

        BeginDrawing();
        ClearBackground(Color{20, 25, 46, 255});

        sceneManager.RenderFrame();

        EndDrawing();
    }
}

void Engine::Shutdown() {
    CloseWindow();
}
