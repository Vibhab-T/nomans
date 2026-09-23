#include "Game.h"
#include "Mouse.h"
#include "raylib/raylib.h"

void Game::updateCamera(const float dt) {
  // dt unused, c++ magic to let it know its unused
  (void)dt; // or something

  // freefly
  if (!m_cameraTarget) {
    if (Mouse::isCaptured()) {
      UpdateCamera(&m_camera, CAMERA_FREE);
    }
    return;
  }

  const Vector3 &target = m_cameraTarget->cTransform->position;
  m_camera.target = target;

  m_camera.position = (Vector3){target.x, target.y, target.z + 100.f};
}

void Game::changeCameraTarget(const std::shared_ptr<Entity> &entity) {
  // entity might be null
  m_cameraTarget = entity;
}

void Game::cycleCameraTarget() {
  const auto &bodies = m_entities.getEntities("celestial");

  if (bodies.empty()) {
    changeCameraTarget(nullptr);
    return;
  }

  if (!m_cameraTarget) {
    changeCameraTarget(bodies.front());
    return;
  }

  for (std::size_t i = 0; i < bodies.size(); i++) {
    if (bodies[i]->id() == m_cameraTarget->id()) {
      // if same, change to next, and none after last
      changeCameraTarget(i + 1 < bodies.size() ? bodies[i + 1] : nullptr);
      return;
    }
  }

  // fallback
  changeCameraTarget(nullptr);
}
