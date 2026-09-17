#include "Icosphere.h"
#include "raylib/raylib.h"

int main() {
  const int screenWidth = 1000;
  const int screenHeight = 700;

  InitWindow(screenWidth, screenHeight, "Ico");

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
    UpdateCamera(&camera, CAMERA_FREE);
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
    }
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
