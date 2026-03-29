//
// Created by mateu on 29.03.2026.
//

#include "core/SceneManager.h"

SceneManager::SceneManager() {
    root = std::make_unique<Node>();
    root->name = "Root";
}

void SceneManager::ProcessFrame(float dt) {
    ProcessNode(root.get(), dt);
}

void SceneManager::RenderFrame() {
    RenderNode(root.get());
}

void SceneManager::ProcessNode(Node* node, float dt) {
    if (!node || !node->isActive) return;

    node->_Process(dt);

    for (auto& child : node->children) {
        ProcessNode(child.get(), dt);
    }
}

void SceneManager::RenderNode(Node* node) {
    if (!node || !node->isActive) return;

    node->_Render();

    for (auto& child : node->children) {
        RenderNode(child.get());
    }
}
