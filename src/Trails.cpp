#include "Game.h"

void Game::updateTrails() {
  if (!m_trailsEnabled) {
    return;
  }

  for (auto &body : m_entities.getEntities("celestial")) {
    auto &points = body->cTrail->points;
    points.push_back(body->cTransform->position);
    if (points.size() > CTrail::maxPoints) {
      points.erase(points.begin());
    }
  }
}
