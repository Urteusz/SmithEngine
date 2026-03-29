#include "input/InputManager.h"

void InputManager::Bind(int key, const std::string& action) {
    bindings[action] = key;
}

bool InputManager::IsPressed(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return IsKeyPressed(it->second);
}

bool InputManager::IsDown(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return IsKeyDown(it->second);
}

bool InputManager::IsReleased(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return IsKeyReleased(it->second);
}

Vector2 InputManager::MouseDelta() const {
    return GetMouseDelta();
}

void InputManager::SetMouseLocked(bool locked) {
    if (locked) {
        DisableCursor();
    } else {
        EnableCursor();
    }
}
