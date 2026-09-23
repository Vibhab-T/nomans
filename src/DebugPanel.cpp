#include "Game.h"
#include "Mouse.h"
#include "raylib/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raylib/raygui.h"

void Game::sUi() {
  //always draw
  DrawFPS(10, 10);
  DrawText("Press TAB to open the debug panel", 10, 30, 20, GREEN);
  if (m_paused) {
    DrawText("PAUSED", 10, 50, 20, RED);
  }

  if (Mouse::isCaptured()) {
    return;
  }
  //draw only if mouse released
  GuiPanel({1520, 0, 400, 1080}, "UI Panel");

  static char cameraHint[] = "Press C to cycle camera target";
  GuiTextBox({1550, 100, 300, 100}, cameraHint, sizeof(cameraHint), false);

  static char panelHint[] = "Press TAB to close this panel";
  GuiTextBox({1550, 250, 300, 100}, panelHint, sizeof(panelHint), false);
}
