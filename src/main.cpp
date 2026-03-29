#include "core/Engine.h"
#include "nodes/Node3D.h"
#include "input/InputManager.h"
#include "raylib.h"

class DemoScene : public Node {
public:
    Camera3D camera{};
    InputManager* input = nullptr;

    void _Ready() override {
        camera.position = Vector3{4.0f, 4.0f, 4.0f};
        camera.target   = Vector3{0.0f, 0.0f, 0.0f};
        camera.up       = Vector3{0.0f, 1.0f, 0.0f};
        camera.fovy     = 45.0f;
        camera.projection = CAMERA_PERSPECTIVE;
    }

    void _Process(float dt) override {
        UpdateCamera(&camera, CAMERA_ORBITAL);

        if (input && input->IsPressed("reset_camera")) {
            camera.position = Vector3{4.0f, 4.0f, 4.0f};
            camera.target   = Vector3{0.0f, 0.0f, 0.0f};
        }
    }

    void _Render() override {
        BeginMode3D(camera);
        DrawCube(Vector3{0.0f, 0.0f, 0.0f}, 2.0f, 2.0f, 2.0f, RED);
        DrawCubeWires(Vector3{0.0f, 0.0f, 0.0f}, 2.0f, 2.0f, 2.0f, MAROON);
        EndMode3D();

        DrawRectangle(20, 20, 200, 100, BLUE);
        DrawText("Dwuwymiarowy obiekt", 30, 60, 16, RAYWHITE);
    }
};

int main() {
    Engine engine;
    engine.Init(1280, 720, "SmithEngine — Demo", 60);

    engine.GetInputManager().Bind(KEY_SPACE, "reset_camera");

    auto demo = std::make_unique<DemoScene>();
    demo->name = "DemoScene";
    demo->input = &engine.GetInputManager();
    /** Move(demo) przenosi własność obiektu do sceny, nie da się inaczej, bo używamy unique_ptr */
    engine.GetSceneManager().GetRoot()->AddChild(std::move(demo));

    engine.Run();
    engine.Shutdown();

    return 0;
}
