#include "core/Engine.h"
#include "core/Scene3D.h"
#include "nodes/Node3D.h"
#include "nodes/MeshNode.h"

#ifdef SMITH_DEBUG
#pragma message("SMITH_DEBUG jest zdefiniowane więc imgui będzie działać")
#else
#pragma message("SMITH_DEBUG nie jest zdefiniowane - imgui off.")
#endif

class TestPlane : public Node3D {
public:
    void _Render() override {
        DrawPlane(transform.position, {50.f, 50.f}, GREEN);
    }
};

class TestLabel : public Node {
public:
    Scene3D* sceneParam = nullptr;
    std::string hitTargetName = "None";

    void _Process(float dt) override {
        if (!sceneParam) return;

        Vector2 screenCenter = { (float)GetScreenWidth() / 2.0f, (float)GetScreenHeight() / 2.0f };
        Ray ray = GetMouseRay(screenCenter, sceneParam->camera);

        float minDistance = 10000.0f;
        hitTargetName = "None";

        // Sprawdzamy kolizję z każdym node'm w scenie
        for (auto& child : sceneParam->children) {
            MeshNode* meshNode = dynamic_cast<MeshNode*>(child.get());
            if (meshNode && meshNode->isActive) {
                // Model może mieć wiele meshy, sprawdzamy każdy z nich
                for (int i = 0; i < meshNode->resource->model.meshCount; i++) {
                    RayCollision col = GetRayCollisionMesh(ray, meshNode->resource->model.meshes[i], meshNode->GetWorldTransform());
                    if (col.hit && col.distance < minDistance) {
                        minDistance = col.distance;
                        hitTargetName = meshNode->name;
                    }
                }
            }
        }
    }

    void _Render() override {
        DrawRectangle(20, 20, 250, 60, Fade(BLUE, 0.8f));
        DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 30, 30, 16, RAYWHITE);
        DrawText(("Hit: " + hitTargetName).c_str(), 30, 50, 16, RAYWHITE);

        // Rysowanie crosshaira na środku ekranu
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();
        DrawCircle(screenWidth / 2, screenHeight / 2, 4, BLACK);
    }
};

int main() {
    Engine engine;
    engine.Init(1280, 720, "SmithEngine — Demo", 60);

    auto scene = std::make_unique<Scene3D>();
    scene->name = "DemoScene";

    auto anvil = std::make_unique<MeshNode>("assets/models/anvil.obj");
    anvil->name = "Anvil";

    scene->AddChild(std::move(anvil));

    auto plane = std::make_unique<TestPlane>();
    plane->name = "TestPlane";
    plane->transform.position.y = -1.0f;
    scene->AddChild(std::move(plane));

    auto label = std::make_unique<TestLabel>();
    label->sceneParam = scene.get(); // Przekazujemy wskaźnik na scenę
    label->name = "DebugLabel";
    scene->hud.AddChild(std::move(label));

    /** Move(scene) przenosi własność obiektu do sceny, nie da się inaczej, bo używamy unique_ptr */
    engine.GetSceneManager().GetRoot()->AddChild(std::move(scene));

    engine.Run();
    engine.Shutdown();

    return 0;
}
