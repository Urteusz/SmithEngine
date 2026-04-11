#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include "core/Node.h"

class SceneManager {
public:
    SceneManager();
    ~SceneManager() = default;

    Node* GetRoot() { return root.get(); }

    void FixedProcessFrame(float fixedDt);
    void ProcessFrame(float dt);
    void RenderFrame();

#ifdef SMITH_DEBUG
    void RenderDebugUI();
#endif

private:
    std::unique_ptr<Node> root;

    void FixedProcessNode(Node* node, float fixedDt);
    void ProcessNode(Node* node, float dt);
    void RenderNode(Node* node);

#ifdef SMITH_DEBUG
    Node* selectedNode = nullptr;
    void RenderDebugNode(Node* node, int depth);
    void RenderInspector();
#endif
};

#endif //SCENEMANAGER_H
