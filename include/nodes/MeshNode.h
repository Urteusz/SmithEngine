#pragma once
#include "nodes/Node3D.h"
#include "core/Engine.h"
#include "raylib.h"

class MeshNode : public Node3D {
public:
    ModelResource* resource = nullptr;
    Color tint = RED; //Tymaczsowo

    explicit MeshNode(const char* path) : _path(path) {
        resource = Engine::Get().modelsManager.get(path);

        if (!resource) {
            resource = new ModelResource{ LoadModel(path), 0 };
            Engine::Get().modelsManager.insert(path, resource);
        }

        // Zliczamy licznik wystąpień (przyda się).
        resource->refCount++;
    }

    ~MeshNode() override {
        if (resource && --resource->refCount == 0) {
            UnloadModel(resource->model);
            Engine::Get().modelsManager.remove(_path);
            delete resource;
        }
    }

    void _Render() override {
        resource->model.transform = transform.GetLocalMatrix();
        DrawModel(resource->model, {0, 0, 0}, 1.0f, tint);
        DrawModelWires(resource->model, {0, 0, 0}, 1.0f, BLACK);
    }

private:
    const char* _path = nullptr;
};
