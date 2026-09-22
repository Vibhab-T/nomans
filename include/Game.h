#pragma once

#include "ecs/Entity.h"
#include "ecs/EntityManager.h"

class Game {
  EntityManager m_entities;
  Camera3D m_camera{};

  int m_currFrame{0};

  bool m_paused{false};
  bool m_running{true};

  bool m_trailsEnabled{true};

  void init();

  void setPaused();

  // game systems
  void sUserInput();
  void sRender();
  void sUi();

  // game functions
  void resetGame();

  // here now we go
  void spawnCelestialBody(const float m, const Vector3 &v, const Vector3 &p, const float r, const Color c);
  void editCelestialBody();

  // these methods below idk if should be here or on the ui sys
  void changeCameraTarget(
      // pass a refercne to the entiry to target to?
  );

  //updates
  void updateCelestialBodies(const float dt); //here is where everything should happen 
  void updateTrails();

public:
  Game();
  void run();
};
