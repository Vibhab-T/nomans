#pragma once

#include "raylib/raylib.h"

namespace Mouse {
void update();
bool isCaptured();
void setCapture(bool captured);
Vector3 position();
} // namespace Mouse
