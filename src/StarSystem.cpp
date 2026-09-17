#include "StarSystem.h"
#include <cmath>

StarSystem::StarSystem(uint8_t planetNums) {
  planets.reserve(planetNums);
  for (std::size_t i = 0; i < planetNums; i++) {
    planets.emplace_back(4);

    planets.back().position = {static_cast<float>(i + 2) * 5.0f, 0.0f, 0.0f};
  }
}

StarSystem::~StarSystem() {}

void StarSystem::update() {
  static float angle = 0.0f;

  angle += 0.01f;

  for (std::size_t i = 0; i < planets.size(); i++) {
    float radius = static_cast<float>(i + 2) * 5.0f;
    float planetAngle = angle + (2.0f * PI * i / planets.size());

    planets[i].position = {std::cos(planetAngle) * radius, 0.0f,
                           std::sin(planetAngle) * radius};
  }
}
