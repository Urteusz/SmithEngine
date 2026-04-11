#pragma once
#include "core/Node.h"
#include "raylib.h"

/** Bazowa klasa dla elementów HUD/2D. Przechowuje pozycję w pikselach ekranu. */
class Node2D : public Node {
public:
    Vector2 position{0.0f, 0.0f};
    Vector2 size{0.0f, 0.0f};
};
