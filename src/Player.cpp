#include "CPlayerState.h"
#include "Game.h"
#include "Icosphere.h"
#include "ecs/Components.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include <memory>

namespace {
// units (m) per second
constexpr float kPlayerSpeed = 30.f;
} // namespace

void Game::spawnPlayer(const Vector3 &position) {
  auto pEntity = m_entities.addEntity("player");

  pEntity->cTransform =
      std::make_shared<CTransform>(position, Vector3(0.f, 0.f, 0.f), 0.f, 0.5f);
  pEntity->cPlayerState = std::make_shared<CPlayerState>();
  pEntity->cPlayerInput = std::make_shared<CPlayerInput>();

  Model playerModel = Icosphere::generate(0);
  pEntity->cModel = std::make_shared<CModel>(playerModel, BLUE);
}

void Game::updatePlayer(const float dt) {
  auto &player = m_entities.getEntities("player").front();
  const auto &input = player->cPlayerInput;

  // The camera looks down -Z with +Y up, so:
  //   W = -Z (away from camera), S = +Z
  //   A = -X (screen left),      D = +X
  //   SPACE = +Y,                CTRL = -Y
  Vector3 direction{0.f, 0.f, 0.f};
  if (input) {
    direction.z = (input->backward ? 1.f : 0.f) - (input->forward ? 1.f : 0.f);
    direction.x = (input->right ? 1.f : 0.f) - (input->left ? 1.f : 0.f);
    direction.y = (input->up ? 1.f : 0.f) - (input->down ? 1.f : 0.f);
  }

  // normalize so diagonals aren't faster (raymath returns zero for zero length)
  direction = Vector3Normalize(direction);
  player->cTransform->velocity = Vector3Scale(direction, kPlayerSpeed);

  // update position last;
  player->cTransform->position =
      Vector3Add(player->cTransform->position,
                 Vector3Scale(player->cTransform->velocity, dt));
}
