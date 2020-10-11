//
// Created by Pavel Gnusin on 04.10.2020.
//

#ifndef ENTITYMANGER_H
#define ENTITYMANGER_H

#include "./Entity.h"
#include "./Component.h"
#include <vector>

class EntityManager {
private:
    std::vector<Entity*> entities;
public:
    void ClearData();
    void Update(float deltaTime);
    void Render();
    bool HasNoEntities();
    Entity& AddEntity(std::string entityName);
    std::vector<Entity*> GetEntities() const;
    unsigned int GetEntityCount();
    void ListAllEntities() const;
};

#endif //ENTITYMANGER_H
