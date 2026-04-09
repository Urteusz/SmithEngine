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
        DisableCursor();
    }

    void _Process(float dt) override {
        Vector3 movement = { 0.0f, 0.0f, 0.0f };
        Vector3 rotation = { 0.0f, 0.0f, 0.0f };

        float speed = 10.0f * dt;
        float sensitivity = 0.05f;

        if (IsKeyDown(KEY_W)) movement.x += speed;
        if (IsKeyDown(KEY_S)) movement.x -= speed;
        if (IsKeyDown(KEY_D)) movement.y += speed;
        if (IsKeyDown(KEY_A)) movement.y -= speed;
        if (IsKeyDown(KEY_SPACE)) movement.z += speed;
        if (IsKeyDown(KEY_LEFT_CONTROL)) movement.z -= speed;

        Vector2 mouseDelta = GetMouseDelta();
        rotation.x = mouseDelta.x * sensitivity;
        rotation.y = mouseDelta.y * sensitivity;

        UpdateCameraPro(&camera, movement, rotation, 0.0f);

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