#include "Icosphere.h"
#include "raylib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raylib/raygui.h"

#include "Mouse.h"

int main() {
  const int screenWidth = 1920;
  const int screenHeight = 1080;

  InitWindow(screenWidth, screenHeight, "Ico");
  Mouse::setCapture(true);

  // Camera
  Camera3D camera = {};
  camera.position = (Vector3){10.f, 10.f, 10.f};
  camera.target = (Vector3){0, 0, 0};
  camera.up = (Vector3){0, 1, 0};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  // PLANETS
  Icosphere sphere = Icosphere(0);
  sphere.position = {0, 0, 0};
  Icosphere sphereTwo = Icosphere(2);
  sphereTwo.position = {0, 0, 5};

  DisableCursor();

  SetTargetFPS(60);

  while (!WindowShouldClose()) {

    // UPDATES
    Mouse::update();
    if (Mouse::isCaptured()) {
      UpdateCamera(&camera, CAMERA_FREE);
    }

    if (IsKeyPressed(KEY_Z))
      camera.target = (Vector3){0, 0, 0};

    BeginDrawing();
    {
      ClearBackground(WHITE);

      BeginMode3D(camera);
      {
        DrawGrid(100, 1.f);
        DrawModel(sphere.model, sphere.position, 1, GREEN);
        DrawModel(sphereTwo.model, sphereTwo.position, 1, RED);
      }
      EndMode3D();
      GuiPanel((Rectangle){1700, 0, 220, 1080}, "Ico Debug Panel");
    }
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
