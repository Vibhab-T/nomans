#pragma once

#include "CCelestialBody.h"
#include "CPlanet.h"
#include "Components.h"

#include <cstddef>
#include <memory>
#include <string>

class Entity {
  friend class EntityManager;

  bool m_active{true};
  size_t m_id{0};
  std::string m_tag{"defualt"};

  Entity(const size_t id, const std::string &tag) : m_id(id), m_tag(tag) {}

public:
  // component pointers
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CModel> cModel;
  std::shared_ptr<CRoundCollision> cRoundCollision;
  std::shared_ptr<CPlanet> cPlanet;
  std::shared_ptr<CCelestialBody> cCelestialBody;

  bool isActive() const;
  const std::string &tag() const;
  size_t id() const;
  void destroy();
};
