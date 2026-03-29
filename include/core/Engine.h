#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#include "core/SceneManager.h"
#include "input/InputManager.h"

class Engine {
public:
    Engine() = default;
    ~Engine() = default;

    void Init(int width, int height, const std::string& title, int targetFps = 60);
    void Run();
    void Shutdown();

    SceneManager& GetSceneManager() { return sceneManager; }
    InputManager& GetInputManager() { return inputManager; }

private:
    SceneManager sceneManager;
    InputManager inputManager;
};

#endif //ENGINE_H
