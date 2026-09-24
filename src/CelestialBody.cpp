#include "CCelestialBody.h"
#include "Game.h"
#include "Icosphere.h"
#include "Physics.h"
#include "ecs/Components.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include <memory>

void Game::spawnCelestialBody(const float mass, const Vector3 &velocity,
                              const Vector3 &position, const float radius,
                              const Color color) {
  auto celestialEntity = m_entities.addEntity("celestial");
  celestialEntity->cCelestialBody = std::make_shared<CCelestialBody>(mass);
  celestialEntity->cTransform = std::make_shared<CTransform>(
      // position (m)
      position,
      // velocity (m/s)
      velocity,
      // angle
      0.f,
      // scale: Icosphere::generate builds a unit sphere (radius 1m), and
      // DrawModel's scale multiplies that directly, so passing radius here
      // renders the body as an actual sphere of that radius in meters.
      radius);
  Model celestialModel = Icosphere::generate(3);
  celestialEntity->cModel = std::make_shared<CModel>(celestialModel, color);
  celestialEntity->cTrail = std::make_shared<CTrail>();
}

void Game::updateCelestialBodies(const float dt) {
  constexpr float kSofteningFactor = 20.5f;
  const auto &bodies = m_entities.getEntities("celestial");

  // first update velocity
  for (auto &body : bodies) {

    for (auto &other : bodies) {
      if (body->id() == other->id()) {
        continue;
      }
      Vector3 direction = Vector3Subtract(other->cTransform->position,
                                          body->cTransform->position);

      float sqrDist = Vector3LengthSqr(direction) + kSofteningFactor;

      Vector3 forceDirection = Vector3Normalize(direction);

      float forceMagnitude = Physics::G * body->cCelestialBody->mass *
                             other->cCelestialBody->mass / sqrDist;

      Vector3 force = Vector3Scale(forceDirection, forceMagnitude);

      Vector3 acceleration =
          Vector3Scale(force, 1.0f / body->cCelestialBody->mass);

      body->cTransform->velocity = Vector3Add(body->cTransform->velocity,
                                              Vector3Scale(acceleration, dt));
    }
  }

  // then update the positions
  for (auto &body : bodies) {
    body->cTransform->position =
        Vector3Add(body->cTransform->position,
                   Vector3Scale(body->cTransform->velocity, dt));
  }
}
