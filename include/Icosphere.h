#pragma once

#include "raylib/raylib.h"

#include <array>
#include <cstdint>

class Icosphere {

  static constexpr Vector3 v0 = {0.8506508f, 0.5257311f, 0.f};
  static constexpr Vector3 v1 = {0.000000101405476f, 0.8506507f, -0.525731f};
  static constexpr Vector3 v2 = {0.000000101405476f, 0.8506506f, 0.525731f};
  static constexpr Vector3 v3 = {0.5257309f, -0.00000006267203f, -0.85065067f};
  static constexpr Vector3 v4 = {0.52573115f, -0.00000006267203f, 0.85065067f};
  static constexpr Vector3 v5 = {0.8506508f, -0.5257311f, 0.f};
  static constexpr Vector3 v6 = {-0.52573115f, 0.00000006267203f, -0.85065067f};
  static constexpr Vector3 v7 = {-0.8506508f, 0.5257311f, 0.f};
  static constexpr Vector3 v8 = {-0.5257309f, 0.00000006267203f, 0.85065067f};
  static constexpr Vector3 v9 = {-0.000000101405476f, -0.8506506f, -0.525731f};
  static constexpr Vector3 v10 = {-0.000000101405476f, -0.8506507f, 0.525731f};
  static constexpr Vector3 v11 = {-0.8506508f, -0.5257311f, 0.f};

  static constexpr std::array<Vector3, 12> vertices = {
      v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11};

  static constexpr std::array<uint32_t, 60> triangleFacesIndices = {
      0, 1,  2, 0, 3,  1, 0, 2,  4, 3,  0, 5,  0, 4, 5, 1,  3,  6,  1,  7,
      2, 7,  1, 6, 4,  2, 8, 7,  8, 2,  9, 3,  5, 6, 3, 9,  5,  4,  10, 4,
      8, 10, 9, 5, 10, 7, 6, 11, 7, 11, 8, 11, 6, 9, 8, 11, 10, 10, 11, 9};

public:
  Mesh mesh{};
  Model model{};
  uint8_t fragDepth;
  Vector3 position{0, 0, 0};
  Color modelColor;
  Color wireColor;

  Icosphere(uint8_t fd, Color mc = YELLOW, Color wc = GREEN);
  ~Icosphere();

  void setFragmentDepth(uint8_t fd);

private:
  void m_fragment(uint8_t fragDepth);
  static Vector3 m_slerp(Vector3 p0, Vector3 p1, float t = 0.5);
};
