//
// Created by mateu on 07.04.2026.
//
#pragma once
#include "core/Scene.h"
#include "raylib.h"

#ifndef SCENE3D_H
#define SCENE3D_H


/** Każda scena 3D ma już stworzoną klasę, żeby nie pisać tego dziesięć razy
 * Startujemy z zainicjalizowaną kamerą (PRZY OVERRIDE _READY pamiętaj o Scene3D::_Ready()!)
 * i Nodem na HUD, który jest renderowany oddzielnie od reszty sceny, więc można tam wrzucać elementy interfejsu.
 */
class Scene3D : public Scene {
public:
    Camera3D camera{};
    Node hud;

    void _Ready() override {
        camera.position   = Vector3{4.0f, 4.0f, 4.0f};
        camera.target     = Vector3{0.0f, 0.0f, 0.0f};
        camera.up         = Vector3{0.0f, 1.0f, 0.0f};
        camera.fovy       = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
    }

    void _Process(float dt) override {
        UpdateCamera(&camera, CAMERA_ORBITAL);
        for (auto& child : hud.children) {
            PropagateProcess(child.get(), dt);
        }
    }

    void _Render() override {
        BeginMode3D(camera);
        for (auto& child : children) {
            PropagateRender(child.get());
        }
        EndMode3D();
        for (auto& child : hud.children) {
            PropagateRender(child.get());
        }
    }

private:
    void PropagateProcess(Node* node, float dt) {
        if (!node || !node->isActive) return;
        node->_Process(dt);
        for (auto& child : node->children) {
            PropagateProcess(child.get(), dt);
        }
    }

    void PropagateRender(Node* node) {
        if (!node || !node->isActive) return;
        node->_Render();
        for (auto& child : node->children) {
            PropagateRender(child.get());
        }
    }
};

#endif //SCENE3D_H