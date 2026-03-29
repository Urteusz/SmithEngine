#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <memory>
#include "core/Node.h"

class SceneManager {
public:
    SceneManager();
    ~SceneManager() = default;

    Node* GetRoot() { return root.get(); }

    void ProcessFrame(float dt);
    void RenderFrame();

private:
    std::unique_ptr<Node> root;

    void ProcessNode(Node* node, float dt);
    void RenderNode(Node* node);
};

#endif //SCENEMANAGER_H
