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

  //game systems
  void sUserInput();
  void sRender();
  void sUi();

  //game functions
  void resetGame();
  void spawnSystem();

  //here now we go
  void spawnCelestialBody(); 
  void addCelestialBody(); //this mniggt be the same as spawnCB
  void editCelestialBody();
  
  //these methods below idk if should be here or on the ui sys
  void toggleTrail();
  void changeCameraTarget(
    //pass a refercne to the entiry to target to?
  );


 

  void updateSystem(const float dt);

public:
  Game();
  void run();
};
