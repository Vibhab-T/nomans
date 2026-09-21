#include "CCelestialBody.h"
#include "Game.h"
#include "Icosphere.h"
#include "ecs/Components.h"
#include "raylib/raylib.h"
#include <memory>

void Game::spawnCelestialBody(const float m, const Vector3 &v,
                              const Vector3 &p) {
  auto celestialEntity = m_entities.addEntity("celestial");
  celestialEntity->cCelestialBody = std::make_shared<CCelestialBody>(m, v);
  celestialEntity->cTransform = std::make_shared<CTransform>(
      // position
      p,
      // velocity
      v,
      // angle
      0.f,
      // scale,
      1.f // need to make this something else to make spawning bodies a bit
          // intuitive like planetRadius or something
  );
  Model celestialModel = Icosphere::generate(3);
  celestialEntity->cModel = std::make_shared<CModel>(celestialModel, GREEN);
};

// the velocity updating would be enough i think.
void Game::updateCelestialBodies() {
  for (auto &celestial : m_entities.getEntities("celestial")) {
    for (auto &nextCelestial : m_entities.getEntities("celestial")) {
      if (celestial->id() == nextCelestial->id()) {
        continue;
      }
      // calculate the force and store/append it in celestial's
      // forcesActingOnBody
      // here is the problem now
      // force is a vector
      // so we have to calculate it in 3 dimension?
      // or use the formula and multiply with a direction vectoru
    }
  }
}
