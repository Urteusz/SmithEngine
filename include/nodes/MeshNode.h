#pragma once
#include "nodes/Node3D.h"
#include "raylib.h"

class MeshNode : public Node3D {
public:
    Model model{};
    Color tint = RED;

    explicit MeshNode(const char* path) {
        model = LoadModel(path);
    }

    ~MeshNode() override {
        UnloadModel(model);
    }

    void _Render() override {
        DrawModel(model, transform.position, 1.0f, tint);
        DrawModelWires(model, transform.position, 1.0f, BLACK);
    }
};
