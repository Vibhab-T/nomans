#pragma once

#include <memory>

class Entity;

class CPlayerState {
public:
  bool isInJetpack{false};
  bool isWalking{false};
  bool isInSpaceship{false};
  bool isInSpace{false};

  std::weak_ptr<Entity> jetpack;
  std::weak_ptr<Entity> spaceship;

  CPlayerState() = default;
};
