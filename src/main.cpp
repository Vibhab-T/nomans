#include "Icosphere.h"
#include "raylib/raylib.h"
#include <cstdint>
#define RAYGUI_IMPLEMENTATION
#include "raylib/raygui.h"

#include "Mouse.h"

void sDebugUiSystem(bool &drawWire, bool &drawModel, uint8_t &fragDepth,
                    uint8_t &scale) {
  GuiPanel((Rectangle){1695, 0, 225, 1080}, "Ico Debug Panel");

  GuiCheckBox((Rectangle){1705, 50, 20, 20}, "Draw Wires", &drawWire);
  GuiCheckBox((Rectangle){1705, 100, 20, 20}, "Draw Model", &drawModel);

  GuiLabel((Rectangle){1705, 150, 100, 20}, "Fragment Depth");
  float fragValue = fragDepth;
  GuiSlider((Rectangle){1710, 170, 180, 20}, "0", "8", &fragValue, 0, 8);
  fragDepth = static_cast<uint8_t>(fragValue);

  GuiLabel((Rectangle){1705, 195, 100, 20}, "Scale");
  float scaleValue = scale;
  GuiSlider((Rectangle){1710, 215, 180, 20}, "0", "10", &scaleValue, 0, 10);
  scale = static_cast<uint8_t>(scaleValue);

  char centerText[] = "Press 'Z' to center the model.";
  char cameraText[] = "Press 'Tab' to toggle camera movement";
  char exitText[] = "Press 'Esc' to exit";

  GuiTextBox(Rectangle(1705, 250, 200, 50), centerText, 20, false);

  GuiTextBox(Rectangle(1705, 310, 200, 50), cameraText, 20, false);

  GuiTextBox(Rectangle(1705, 370, 200, 50), exitText, 20, false);
}

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
  bool drawWire = false;
  bool drawModel = true;
  uint8_t fragDepth = 0;
  uint8_t scale = 1;

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

    sphere.setFragmentDepth(fragDepth);

    BeginDrawing();
    {
      ClearBackground(WHITE);

      BeginMode3D(camera);
      {
        DrawGrid(100, 1.f);
        if (drawWire) {
          DrawModelWires(sphere.model, sphere.position, scale, RED);
        }
        if (drawModel) {
          DrawModel(sphere.model, sphere.position, scale, GREEN);
        }
      }
      EndMode3D();
    }

    sDebugUiSystem(drawWire, drawModel, fragDepth, scale);
    EndDrawing();
  }
  CloseWindow();

  return 0;
}
