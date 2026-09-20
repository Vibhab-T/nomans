#include "Game.h"
#include "Icosphere.h"
#include "ecs/Components.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include <cmath>
#include <memory>
#include <random>
#include <vector>

void Game::spawnSystem() {
  static std::random_device rd;
  static std::mt19937 rng(rd());

  std::uniform_real_distribution<float> revolutionSpeedDist(0.5f, 2.0f);
  std::uniform_real_distribution<float> rotationSpeedDist(1.0f, 5.0f);
  std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * PI);
  std::uniform_real_distribution<float> scaleDist(1.0f, 4.0f);
  std::uniform_real_distribution<float> gapDist(
      4.0f, 10.0f); // extra breathing room between orbits, part of the jadoo
                    // later mentioned
  std::bernoulli_distribution directionDist(0.5);

  // the sun
  auto sun = m_entities.addEntity("sun");
  const float sunScale = 5.5f;

  sun->cTransform = std::make_shared<CTransform>(
      Vector3(0, 0, 0), Vector3(0, 0, 0), 0.f, sunScale);
  Model sunModel = Icosphere::generate(3);
  sun->cModel = std::make_shared<CModel>(sunModel, YELLOW);

  // the planets
  float prevRadius = sunScale; // clear the sun first
  float prevScale = 0.f;

  for (std::size_t i = 0; i < 4; i++) {
    auto planet = m_entities.addEntity("planet");

    float scale = scaleDist(rng);
    float radius = prevRadius + prevScale + scale +
                   gapDist(rng); // the aforementioned jadoo, add shit and hope
                                 // its seperate enough
    float revSpeed = revolutionSpeedDist(rng);
    float rotSpeed = rotationSpeedDist(rng);
    float angle = angleDist(rng);
    bool isClockwise = directionDist(rng);
    Vector3 initPos = {std::cos(angle), 0.f, std::sin(angle)};
    Model planetModel = Icosphere::generate(3);

    planet->cTransform =
        std::make_shared<CTransform>(initPos, Vector3(0, 0, 0), 0.f, scale);
    planet->cModel = std::make_shared<CModel>(planetModel, RED);
    planet->cPlanet = std::make_shared<CPlanet>(radius, isClockwise, revSpeed,
                                                rotSpeed, angle);

    prevRadius = radius;
    prevScale = scale;
    // clean the jadoo
  }
}

void Game::updateSystem(const float dt) {
  auto planets = m_entities.getEntities("planet");

  for (auto &planet : planets) {
    float &distanceFromSun = planet->cPlanet->distanceFromSun;
    bool &isClockwise = planet->cPlanet->isRotatingClockwise;
    float &revSpeed = planet->cPlanet->revolutionSpeed;
    // float &rotSpeed = planet->cPlanet->rotationSpeed;
    auto &pos = planet->cTransform->position;
    float &ang = planet->cPlanet->angleFromCenter;
    auto &vel = planet->cTransform->velocity;

    ang += (isClockwise ? 1.f : -1.f) * revSpeed * dt;

    const Vector3 nextPos = {std::cos(ang) * distanceFromSun, 0.f,
                             std::sin(ang) * distanceFromSun};
    // normally the position atrribute of the CTransform component is calculated
    // by the velocity attribure. here we are doing the opposite because
    // deriving position from velocity in a circular situation requires some
    // maths i am not willing to do this velocity attribute is required because
    // other game systems may make use of the vel.
    vel = dt > 0.f ? Vector3Scale(Vector3Subtract(nextPos, pos), 1.f / dt)
                   : Vector3Zero(); // greater jadoo than the one above -
                                    // because i dont understand yet
    pos = Vector3Add(pos, Vector3Scale(vel, dt));
  }
}
