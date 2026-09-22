#include "CCelestialBody.h"
#include "Game.h"
#include "Icosphere.h"
#include "Physics.h"
#include "ecs/Components.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include <cmath>
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
  const auto &bodies = m_entities.getEntities("celestial");

  // the net gravitational force on every body
  for (auto &body : bodies) {
    Vector3 resultantForce{0.f, 0.f, 0.f};

    for (auto &other : bodies) {
      if (body->id() == other->id()) {
        continue;
      }

      //   F = G * m1 * m2 / r^2
      const float Gm1m2 =
          Physics::G * body->cCelestialBody->mass * other->cCelestialBody->mass;

      const Vector3 diff = Vector3Subtract(other->cTransform->position,
                                           body->cTransform->position);
      const float r = Vector3Length(diff);

      // softening: floor r at the sum of the two radii, so F doesn't spike
      // to infinity as the bodies get close
      // / r_eff = max(r, radius1 + radius2)
      const float minDistance =
          body->cTransform->scale + other->cTransform->scale;
      const float rEff = fmaxf(r, minDistance);
      const float forceMagnitude = Gm1m2 / (rEff * rEff);

      // direction is the r vector's unit vector.
      const Vector3 direction = Vector3Scale(diff, 1.f / r);
      // force vector = direction * magnitude
      const Vector3 force = Vector3Scale(direction, forceMagnitude);

      resultantForce = Vector3Add(resultantForce, force);
    }

    body->cCelestialBody->resultantForce = resultantForce;
  }

  for (auto &body : bodies) {
    //   F = m * a    a = F / m
    const Vector3 acceleration = Vector3Scale(
        body->cCelestialBody->resultantForce, 1.f / body->cCelestialBody->mass);

    //   v = u + a * dt
    body->cTransform->velocity =
        Vector3Add(body->cTransform->velocity, Vector3Scale(acceleration, dt));
    // same for position
    //    p = p + v * dt
    body->cTransform->position =
        Vector3Add(body->cTransform->position,
                   Vector3Scale(body->cTransform->velocity, dt));
  }
}
