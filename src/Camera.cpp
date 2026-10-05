#include "Game.h"

void Game::updateCamera(const float dt) {
  (void)dt;
  auto &player = m_entities.getEntities("player").front();
  Vector3 &playerPosition = player->cTransform->position;
  m_camera.target = playerPosition;
  m_camera.position =
      (Vector3){playerPosition.x, playerPosition.y + 10, playerPosition.z + 30.f};
}
