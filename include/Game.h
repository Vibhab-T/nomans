#pragma once

#include "ecs/Entity.h"
#include "ecs/EntityManager.h"
#include "raylib/raylib.h"
#include <memory>

class Game {
  EntityManager m_entities;
  Camera3D m_camera{};

  int m_currFrame{0};

  bool m_paused{false};
  bool m_running{true};

  bool m_trailsEnabled{true};

  std::shared_ptr<Entity> m_cameraTarget;

  void init();

  void setPaused();

  // game systems
  void sUserInput();
  void sRender();
  void sUi();

  // game functions
  void resetGame();

  // here now we go
  void spawnCelestialBody(const float m, const Vector3 &v, const Vector3 &p,
                          const float r, const Color c);
  void editCelestialBody();

  // these methods below idk if should be here or on the ui sys

  // updates
  void updateCelestialBodies(
      const float dt); // here is where everything should happen
  void updateTrails();

  // camera updates
  void updateCamera(const float dt); // follows m_camerTarget
  void changeCameraTarget(const std::shared_ptr<Entity> &entity);
  void cycleCameraTarget();

public:
  Game();
  void run();
};
