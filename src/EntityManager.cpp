//
// Created by Pavel Gnusin on 04.10.2020.
//

#include <iostream>
#include "./EntityManger.h"
#include "./Collision.h"
#include "./Components/ColliderComponent.h"

void EntityManager::ClearData() {
    for (auto &entity: entities) {
        entity->Destroy();
    }
}

bool EntityManager::HasNoEntities() const {
    return entities.empty();
}

void EntityManager::Update(float deltaTime) {
    for (auto &entity: entities) {
        entity->Update(deltaTime);
    }
    DestroyInactiveEntities();
}

void EntityManager::DestroyInactiveEntities() {
    for (int i = 0; i < entities.size(); ++i) {
        if (!entities[i]->IsActive()) {
            entities.erase(entities.begin() + i);
        }
    }
}

void EntityManager::Render() {
    for (int layerNumber = 0; layerNumber < NUM_LAYERS; ++layerNumber) {
        for (auto &entity: GetEntitiesByLayer(static_cast<LayerType>(layerNumber))) {
            entity->Render();
        }
    }
}

Entity &EntityManager::AddEntity(std::string entityName, LayerType layer) {
    Entity *entity = new Entity(*this, entityName, layer);
    entities.emplace_back(entity);
    return *entity;
}

std::vector<Entity *> EntityManager::GetEntities() const {
    return entities;
}

std::vector<Entity *> EntityManager::GetEntitiesByLayer(LayerType layer) const {
    std::vector<Entity *> selectedEntities;
    for (auto &entity: entities) {
        if (entity->layer == layer) {
            selectedEntities.emplace_back(entity);
        }
    }
    return selectedEntities;
}


unsigned int EntityManager::GetEntityCount() const {
    return entities.size();
}

void EntityManager::ListAllEntities() const {
    unsigned int i = 0;
    for (auto &entity: entities) {
        std::cout << "Entity[" << i << "]: " << entity->name << std::endl;
        entity->ListAllComponents();
        i++;
    }
}

CollisionType EntityManager::CheckEntityCollisions() const {
    for (auto &thisEntity: entities) {
        if (thisEntity->HasComponent<ColliderComponent>()) {
            ColliderComponent *thisCollider = thisEntity->GetComponent<ColliderComponent>();
            for (auto &thatEntity: entities) {
                if (thisEntity->name != thatEntity->name && thatEntity->HasComponent<ColliderComponent>()) {
                    ColliderComponent *thatCollider = thatEntity->GetComponent<ColliderComponent>();
                    if (Collision::CheckRectangleCollision(thisCollider->collider, thatCollider->collider)) {
                        if (thisCollider->colliderTag == "PLAYER" && thatCollider->colliderTag == "ENEMY") {
                            return PLAYER_ENEMY_COLLISION;
                        }
                        if (thisCollider->colliderTag == "PLAYER" && thatCollider->colliderTag == "PROJECTILE") {
                            return PLAYER_PROJECTILE_COLLISION;
                        }
                        if (thisCollider->colliderTag == "ENEMY" &&
                            thatCollider->colliderTag == "FRIENDLY_PROJECTILE") {
                            return ENEMY_PROJECTILE_COLLISION;
                        }
                        if (thisCollider->colliderTag == "PLAYER" && thatCollider->colliderTag == "LEVEL_COMPLETE") {
                            return PLAYER_LEVEL_COMPLETE_COLLISION;
                        }
                    }
                }
            }
        }
    }
    return NO_COLLISION;
}