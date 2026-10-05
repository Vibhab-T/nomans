#include "Game.h"
#include "Mouse.h"

void Game::sUserInput() {
  Mouse::update();

  Mouse::update();

  // the player can be found here after the first frame
  const auto &players = m_entities.getEntities("player");
  if (players.empty()) {
    return;
  }

  const auto &input = players.front()->cPlayerInput;
  if (!input) {
    return;
  }

  input->forward = IsKeyDown(KEY_W);
  input->backward = IsKeyDown(KEY_S);
  input->left = IsKeyDown(KEY_A);
  input->right = IsKeyDown(KEY_D);
  input->up = IsKeyDown(KEY_SPACE);
  input->down = IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL);
}
