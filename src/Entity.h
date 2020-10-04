//
// Created by Pavel Gnusin on 02.10.2020.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>

#include "./EntityManger.h"
#include "./Component.h"

class Component;
class EntityManager;

class Entity {
private:
    EntityManager& manager;
    bool isActive;
    std::vector<Component*> components;

public:
    std::string name;
    Entity(EntityManager& manager);
    Entity(EntityManager& manager, std::string name);
    void Update(float deltaTime);
    void Render();
    void Destroy();
    bool IsActive() const;
};

#endif //ENTITY_H
