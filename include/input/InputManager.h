#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <string>
#include <unordered_map>
#include "raylib.h"

class InputManager {
public:
    void Bind(int key, const std::string& action);

    bool IsPressed(const std::string& action) const;
    bool IsDown(const std::string& action) const;
    bool IsReleased(const std::string& action) const;

    Vector2 MouseDelta() const;
    void SetMouseLocked(bool locked);
    bool IsCursorHidden() const;
    bool IsMouseAvailable() const;

private:
    bool cursorHidden = false;
    std::unordered_map<std::string, int> bindings;
};

#endif //INPUTMANAGER_H
