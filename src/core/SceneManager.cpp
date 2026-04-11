#include "core/SceneManager.h"
#include "core/Scene.h"

#ifdef SMITH_DEBUG
#include "imgui.h"
#include "nodes/Node3D.h"
#include "nodes/Node2D.h"
#include "core/Scene3D.h"
#endif

SceneManager::SceneManager() {
    root = std::make_unique<Node>();
    root->name = "Root";
}

void SceneManager::FixedProcessFrame(float fixedDt) {
    FixedProcessNode(root.get(), fixedDt);
}

/** Zapytacie się po co ProcessFrame skoro to tylko wrapper, a ja powiem że dlatego żeby nie trzeba było wywoływać root.get() w mainie. */
void SceneManager::ProcessFrame(float dt) {
    ProcessNode(root.get(), dt);
}

void SceneManager::RenderFrame() {
    RenderNode(root.get());
}

void SceneManager::FixedProcessNode(Node* node, float fixedDt) {
    if (!node || !node->isActive) return;

    node->_FixedProcess(fixedDt);

    for (auto& child : node->children) {
        FixedProcessNode(child.get(), fixedDt);
    }
}

void SceneManager::ProcessNode(Node* node, float dt) {
    if (!node || !node->isActive) return;

    node->_Process(dt);

    for (auto& child : node->children) {
        ProcessNode(child.get(), dt);
    }
}

#ifdef SMITH_DEBUG
void SceneManager::RenderDebugUI() {

    ImGui::Begin("Scene");
    RenderDebugNode(root.get(), 0);
    ImGui::End();

    RenderInspector();
}

void SceneManager::RenderDebugNode(Node* node, int depth) {
    if (!node) return;

    ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
    if (node->children.empty()) flags |= ImGuiTreeNodeFlags_Leaf;

    ImGui::PushID(node);
    ImGui::Checkbox("##active", &node->isActive);
    ImGui::SameLine();

    bool selected = (selectedNode == node);
    bool open = ImGui::TreeNodeEx(node->name.c_str(), flags | (selected ? ImGuiTreeNodeFlags_Selected : 0));
    if (ImGui::IsItemClicked()) selectedNode = node;

    if (open) {
        for (auto& child : node->children)
            RenderDebugNode(child.get(), depth + 1);

        Scene3D* scene3d = dynamic_cast<Scene3D*>(node);
        if (scene3d) {
            RenderDebugNode(&scene3d->hud, depth + 1);
        }

        ImGui::TreePop();
    }
    ImGui::PopID();
}
void SceneManager::RenderInspector() {
    ImGui::Begin("Inspector");
    if (selectedNode) {
        ImGui::Text("%s", selectedNode->name.c_str());
        ImGui::Separator();

        Node3D* node3d = dynamic_cast<Node3D*>(selectedNode);
        if (node3d) {
            bool changed = false;
            changed |= ImGui::DragFloat3("Position", &node3d->transform.position.x, 0.1f);
            changed |= ImGui::DragFloat3("Rotation", &node3d->transform.rotation.x, 1.0f);
            changed |= ImGui::DragFloat3("Scale",    &node3d->transform.scale.x,    0.01f);
            if (changed) node3d->transform.isDirty = true;
        }

        Node2D* node2d = dynamic_cast<Node2D*>(selectedNode);
        if (node2d) {
            ImGui::DragFloat2("Position", &node2d->position.x, 1.0f);
            ImGui::DragFloat2("Size", &node2d->size.x, 1.0f, 0.f,5000.f);
        }
    } else {
        ImGui::TextDisabled("Nic nie wybrano");
    }
    ImGui::End();
}
#endif

void SceneManager::RenderNode(Node* node) {
    if (!node || !node->isActive) return;

    node->_Render();

    // Jeśli to scena — ona sama renderuje swoje dzieci
    if (dynamic_cast<Scene*>(node)) return;

    for (auto& child : node->children) {
        RenderNode(child.get());
    }
}
