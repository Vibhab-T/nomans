#include "Game.h"
#include "Icosphere.h"

void Game::spawnSystem() {
  // the sun
  auto sun = m_entities.addEntity("sun");

  sun->cTransform = std::make_shared<CTransform>(Vector3(0, 0, 0),
                                                 Vector3(0, 0, 0), 0.f, 2.5f);
  Model sunModel = Icosphere::generate(3);
  sun->cModel = std::make_shared<CModel>(sunModel, YELLOW);

  // the planets
  for (std::size_t i = 0; i < 4; i++) {
    auto planet = m_entities.addEntity("planet");

    float radius = (i * 2) + i + 10.0f;

    planet->cTransform = std::make_shared<CTransform>(
        Vector3(radius, 0, 0), Vector3(0, 0, 0), 0.f, 1.0f);
    Model planetModel = Icosphere::generate(3);
    planet->cModel = std::make_shared<CModel>(planetModel, RED);
    planet->cPlanet = std::make_shared<CPlanet>(radius, true);
  }
}

void Game::updateSystem() {
  auto planets = m_entities.getEntities("planets");

  for (auto &planet : planets) {
    auto &pos = planet->cTransform->position;
  }
}
