#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>

class Node {
public:
    std::string name;
    Node* parent = nullptr;
    std::vector<std::unique_ptr<Node>> children;
    bool isActive = true;

    Node() = default;
    virtual ~Node() = default;

    virtual void _Ready() {}
    virtual void _Process(float dt) {}
    virtual void _Render() {}
    virtual void _OnDestroy() {}

    void AddChild(std::unique_ptr<Node> child);
    Node* GetChild(const std::string& childName);
    void RemoveChild(const std::string& childName);

    /** Zrobione po to, żeby można było automatycznie złapało podtyp Node-a */
    template<typename T>
    T* GetChild(const std::string& childName) {
        return dynamic_cast<T*>(GetChild(childName));
    }
};

#endif //NODE_H