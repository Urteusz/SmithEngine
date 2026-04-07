#include "core/Engine.h"
#include "core/Scene3D.h"
#include "nodes/Node3D.h"

// Tymczasowy węzeł testowy — docelowo zastąpi go prawdziwy mesh
class TestCube : public Node3D {
public:
    void _Render() override {
        DrawCube(transform.position, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(transform.position, 2.0f, 2.0f, 2.0f, MAROON);
    }
};

// Tymczasowy węzeł HUD testowy
class TestLabel : public Node {
public:
    void _Render() override {
        DrawRectangle(20, 20, 200, 40, BLUE);
        DrawText("SmithEngine dziala!", 30, 30, 16, RAYWHITE);
    }
};

int main() {
    Engine engine;
    engine.Init(1280, 720, "SmithEngine — Demo", 60);

    auto scene = std::make_unique<Scene3D>();
    scene->name = "DemoScene";

    scene->AddChild(std::make_unique<TestCube>());

    scene->hud.AddChild(std::make_unique<TestLabel>());

    /** Move(scene) przenosi własność obiektu do sceny, nie da się inaczej, bo używamy unique_ptr */
    engine.GetSceneManager().GetRoot()->AddChild(std::move(scene));

    engine.Run();
    engine.Shutdown();

    return 0;
}
