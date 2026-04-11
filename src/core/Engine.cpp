#include "core/Engine.h"
#include "raylib.h"

Engine* Engine::_instance = nullptr;

// Linkujemy tylko jak jesteśmy w trybie debugu
#ifdef SMITH_DEBUG
#include "rlImGui.h"
#endif

void Engine::Init(int width, int height, const std::string& title, int targetFps) {
    InitWindow(width, height, title.c_str());
    SetTargetFPS(targetFps);

#ifdef SMITH_DEBUG
    rlImGuiSetup(true);
#endif
}

void Engine::Run() {
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        sceneManager.ProcessFrame(dt);

        BeginDrawing();
        ClearBackground(Color{20, 25, 46, 255});

#ifdef SMITH_DEBUG
        rlImGuiBegin();
#endif

        sceneManager.RenderFrame();

#ifdef SMITH_DEBUG
        sceneManager.RenderDebugUI();
        rlImGuiEnd();
#endif

        EndDrawing();
    }
}

void Engine::Shutdown() {
#ifdef SMITH_DEBUG
    rlImGuiShutdown();
#endif
    CloseWindow();
}
