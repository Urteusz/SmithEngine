#ifndef ENGINE_H
#define ENGINE_H

#include <string>

#include "core/ResourceManager.h"
#include "core/SceneManager.h"
#include "input/InputManager.h"

class Engine {
    // Zrobiliśmy z engine singletona (z wiadomych przyczyn).
    static Engine* _instance;
public:
    // Nowość ResourceManager jako hash table.
    ResourceManager<ModelResource,128,128> modelsManager;

    Engine() { _instance = this; }
    // Aby odnieść się do silnika to robimy tak Engine::Get() nie tworzymy następnego obiektu.
    static Engine& Get() {return *_instance; }
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
