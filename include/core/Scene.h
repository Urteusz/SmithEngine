//
// Created by mateu on 07.04.2026.
//
#pragma once
#include "core/Node.h"

#ifndef SCENE_H
#define SCENE_H
class Scene : public Node {
public:
    virtual void _OnEnter() {}   // możemy tutaj załaduj assety/odpalić muzykę
    virtual void _OnExit() {}  // tutaj czyścimy stan i zatrzymujemy muzykę
};
#endif //SCENE_H