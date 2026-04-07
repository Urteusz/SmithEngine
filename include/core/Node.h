#ifndef NODE_H
#define NODE_H

#include <string>
#include <vector>
#include <memory>
#include <typeindex>
#include <unordered_map>

// Klasa komponentu — każdy komponent dziedziczy po tej klasie (skrypty)
class Component {
public:
    class Node* owner = nullptr;

    virtual ~Component() = default;
    virtual void _Ready() {}
    virtual void _Process(float dt) {}
    virtual void _OnDestroy() {}
};

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

    /** Dodaje komponent do node-a. Jeden typ komponentu na node. */
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args) {
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        component->owner = this;
        T* ptr = component.get();
        components[std::type_index(typeid(T))] = std::move(component);
        ptr->_Ready();
        return ptr;
    }

    /** Zwraca komponent danego typu, nullptr jeśli nie istnieje. */
    template<typename T>
    T* GetComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    /** Sprawdza czy node posiada komponent danego typu. */
    template<typename T>
    bool HasComponent() const {
        return components.count(std::type_index(typeid(T))) > 0;
    }

    /** Usuwa komponent danego typu. */
    template<typename T>
    void RemoveComponent() {
        auto it = components.find(std::type_index(typeid(T)));
        if (it != components.end()) {
            it->second->_OnDestroy();
            components.erase(it);
        }
    }

    /** Zrobione po to, żeby można było automatycznie złapało podtyp Node-a */
    template<typename T>
    T* GetChild(const std::string& childName) {
        return dynamic_cast<T*>(GetChild(childName));
    }

    /** Wywołaj w _Process node-a żeby komponenty też dostawały update */
    void _ProcessComponents(float dt);
    void _DestroyComponents();

private:
    /** Dzieki temu że używamy unordered_mapki, mamy pewność że nie odjebiemy maniany i nie dodamy dwa razy jednego komponentu*/
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
};

#endif //NODE_H