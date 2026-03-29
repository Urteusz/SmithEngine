//
// Created by mateu on 29.03.2026.
//

#include "core/Node.h"
#include <algorithm>

void Node::AddChild(std::unique_ptr<Node> child) {
    child->parent = this;
    children.push_back(std::move(child));
    children.back()->_Ready();
}

Node* Node::GetChild(const std::string& childName) {
    for (auto& c : children) {
        if (c->name == childName) return c.get();
    }
    return nullptr;
}

void Node::RemoveChild(const std::string& childName) {
    children.erase(
        std::remove_if(children.begin(), children.end(),
            [&](const std::unique_ptr<Node>& c) {
                if (c->name == childName) {
                    c->_OnDestroy();
                    return true;
                }
                return false;
            }),
        children.end()
    );
}
