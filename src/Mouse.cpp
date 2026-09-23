#include "Mouse.h"
#include "raylib/raylib.h"

namespace Mouse {
bool captured = true;

void update() {
  if (IsKeyPressed(KEY_TAB)) {
    captured = !captured;

    if (captured) {
      DisableCursor();
    } else {
      EnableCursor();
    }
  }
}

bool isCaptured() { return captured; }

void setCapture(bool value) {
  captured = value;
  if (captured) {
    DisableCursor();
  } else {
    EnableCursor();
  }
}

Vector3 position() {
  const Vector2 screenPos = GetMousePosition();
  return (Vector3){screenPos.x, screenPos.y, 0.f};
} // namespace Mouse
} // namespace Mouse
