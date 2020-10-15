//
// Created by Pavel Gnusin on 02.10.2020.
//

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "./Constants.h"
#include "./EntityManger.h"
#include "./Component.h"

class Component;

class EntityManager;

class Entity {
private:
    EntityManager &manager;
    bool isActive;
    std::vector<Component *> components;
    std::map<const std::type_info *, Component *> componentTypeMap;
public:
    std::string name;
    LayerType layer;

    Entity(EntityManager &manager);

    Entity(EntityManager &manager, std::string name);

    Entity(EntityManager &manager, std::string name, LayerType layer);

    void Update(float deltaTime);

    void Render();

    void Destroy();

    bool IsActive() const;

    template<typename T, typename ...TArgs>
    T &AddComponent(TArgs &&... args) {
        T *newComponent(new T(std::forward<TArgs>(args)...));
        newComponent->owner = this;
        components.emplace_back(newComponent);
        componentTypeMap[&typeid(*newComponent)] = newComponent;
        newComponent->Initialize();
        return *newComponent;
    }

    template<typename T>
    T *GetComponent() {
        return static_cast<T *>(componentTypeMap[&typeid(T)]);
    }

    void ListAllComponents() const {
        for (auto mapElement: componentTypeMap) {
            std::cout << "  Components<" << mapElement.first->name() << ">" << std::endl;
        }
    }

    template<typename T>
    bool HasComponents() const {
        return componentTypeMap.count(&typeid(T)) == 1;
    }
};

#endif //ENTITY_H
