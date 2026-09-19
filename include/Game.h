#pragma once

#include "ecs/Entity.h"
#include "ecs/EntityManager.h"

class Game {
  EntityManager m_entities;
  Camera3D m_camera{};

  int m_currFrame{0};

  bool m_paused{false};
  bool m_running{true};

  void init();

  void setPaused();

  void sUserInput();
  void sRender();
  void sUi();

  void resetGame();
  void spawnSystem();


  void updateSystem();

public:
  Game();
  void run();
};
