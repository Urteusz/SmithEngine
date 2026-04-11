#include "input/InputManager.h"

#ifdef SMITH_DEBUG
#include "imgui.h"
#endif

void InputManager::Bind(int key, const std::string& action) {
    bindings[action] = key;
}

static bool IsMouseButton(int code) { return code >= 0 && code <= 6; }

bool InputManager::IsPressed(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return IsMouseButton(it->second) ? IsMouseButtonPressed(it->second) : IsKeyPressed(it->second);
}

bool InputManager::IsDown(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return IsMouseButton(it->second) ? IsMouseButtonDown(it->second) : IsKeyDown(it->second);
}

bool InputManager::IsReleased(const std::string& action) const {
    auto it = bindings.find(action);
    if (it == bindings.end()) return false;
    return IsMouseButton(it->second) ? IsMouseButtonReleased(it->second) : IsKeyReleased(it->second);
}

Vector2 InputManager::MouseDelta() const {
    return GetMouseDelta();
}

void InputManager::SetMouseLocked(bool locked) {
    if (locked) {
        DisableCursor();
#ifdef SMITH_DEBUG
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;
#endif
        cursorHidden = true;
    } else {
        EnableCursor();
#ifdef SMITH_DEBUG
        ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouseCursorChange;
#endif
        cursorHidden = false;
    }
}


bool InputManager::IsCursorHidden() const {
    return cursorHidden;
}

bool InputManager::IsMouseAvailable() const {
#ifdef SMITH_DEBUG
    return !ImGui::GetIO().WantCaptureMouse;
#else
    return true;
#endif
}
