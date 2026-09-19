#pragma once

#include "raylib/raylib.h"

class CTransform {
public:
  Vector3 position{0.f, 0.f, 0.f};
  Vector3 velocity{0.f, 0.f, 0.f};
  float angle{0.f};
  float scale{0.f};

  CTransform(const Vector3 &p, const Vector3 &v, float a, float s)
      : position(p), velocity(v), angle(a), scale(s) {};
};

class CModel {
public:
  Model model{};
  Color color{};

  CModel(const Model &m, const Color &c) : model(m), color(c) {}
};

class CRoundCollision {
public:
  float radius{0.f};

  CRoundCollision(float r) : radius(r) {}
};
