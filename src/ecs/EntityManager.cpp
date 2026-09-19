#include "ecs/EntityManager.h"
#include "ecs/Entity.h"
#include <algorithm>
#include <memory>

EntityManager::EntityManager() {}

void EntityManager::update() {
  // add from entities to add
  // add to entity vec
  // add to entity map, tag is key
  for (auto &e : m_entitiesToAdd) {
    m_entities.push_back(e);
    m_entityMap[e->tag()].push_back(e);
  }

  m_entitiesToAdd.clear();

  removeDeadEntities(m_entities);

  for (auto &[tag, vec] : m_entityMap) {
    removeDeadEntities(vec);
  }
}

void EntityManager::removeDeadEntities(EntityVector &vec) {
  vec.erase(std::remove_if(vec.begin(), vec.end(),
                           [](const std::shared_ptr<Entity> &e) {
                             return !e->isActive();
                           }),
            vec.end());
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string &tag) {
  auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntites++, tag));

  m_entitiesToAdd.push_back(entity);

  return entity;
}

const EntityVector &EntityManager::getEntities() { return m_entities; }

const EntityVector &EntityManager::getEntities(const std::string &tag) {
  return m_entityMap[tag];
}
