#pragma once

#include <vector>

#include "raylib/raylib.h"

class CTrail {
public:
  static constexpr std::size_t maxPoints = 500;
  std::vector<Vector3> points;
  // no need for the constructa
};
