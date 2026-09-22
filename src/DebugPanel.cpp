#include "Game.h"
#include "Mouse.h"
#define RAYGUI_IMPLEMENTATION
#include "raylib/raygui.h"

void Game::sUi() {
  if (Mouse::isCaptured()) {
    return;
  }
  GuiPanel({1520, 0, 400, 1080}, "UI Panel");

}
