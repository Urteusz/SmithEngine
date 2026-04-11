//
// Created by mateu on 07.04.2026.
//
#pragma once
#include "core/Scene.h"
#include "input/InputManager.h"
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
    InputManager input;

    void _Ready() override {
        hud.name = "HUD";
        camera.position   = Vector3{4.0f, 4.0f, 4.0f};
        camera.target     = Vector3{0.0f, 0.0f, 0.0f};
        camera.up         = Vector3{0.0f, 1.0f, 0.0f};
        camera.fovy       = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;

        input.Bind(KEY_W,            "move_forward");
        input.Bind(KEY_S,            "move_backward");
        input.Bind(KEY_D,            "move_right");
        input.Bind(KEY_A,            "move_left");
        input.Bind(KEY_SPACE,        "move_up");
        input.Bind(KEY_LEFT_CONTROL, "move_down");
        input.Bind(KEY_H, "toggle_mouse_lock");

        input.SetMouseLocked(true);
    }

    void _Process(float dt) override {
        if (input.IsPressed("toggle_mouse_lock"))
            input.SetMouseLocked(!input.IsCursorHidden());

        if (input.IsCursorHidden() && input.IsMouseAvailable()) {
            Vector3 movement = { 0.0f, 0.0f, 0.0f };
            Vector3 rotation = { 0.0f, 0.0f, 0.0f };

            float speed = 10.0f * dt;
            float sensitivity = 0.05f;

            if (input.IsDown("move_forward"))  movement.x += speed;
            if (input.IsDown("move_backward")) movement.x -= speed;
            if (input.IsDown("move_right"))    movement.y += speed;
            if (input.IsDown("move_left"))     movement.y -= speed;
            if (input.IsDown("move_up"))       movement.z += speed;
            if (input.IsDown("move_down"))     movement.z -= speed;

            Vector2 mouseDelta = input.MouseDelta();
            rotation.x = mouseDelta.x * sensitivity;
            rotation.y = mouseDelta.y * sensitivity;

            UpdateCameraPro(&camera, movement, rotation, 0.0f);
        }

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