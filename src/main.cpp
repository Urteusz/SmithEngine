#include "core/Engine.h"
#include "core/Scene3D.h"
#include "nodes/Node3D.h"
#include "nodes/MeshNode.h"

class TestPlane : public Node3D {
public:
    void _Render() override {
        DrawPlane(transform.position, {10.f, 10.f}, GREEN);
    }
};

class TestLabel : public Node {
public:
    void _Render() override {
        DrawRectangle(20, 20, 200, 40, BLUE);
        DrawText(("FPS: " + std::to_string(GetFPS())).c_str(), 30, 30, 16, RAYWHITE);
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


    scene->hud.AddChild(std::make_unique<TestLabel>());

    /** Move(scene) przenosi własność obiektu do sceny, nie da się inaczej, bo używamy unique_ptr */
    engine.GetSceneManager().GetRoot()->AddChild(std::move(scene));

    engine.Run();
    engine.Shutdown();

    return 0;
}
