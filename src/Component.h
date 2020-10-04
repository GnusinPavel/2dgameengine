//
// Created by Pavel Gnusin on 02.10.2020.
//

#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
public:
    Entity* owner;
    virtual ~Component() = default;
    virtual void Initialize() {}
    virtual void Update(float deltaTime) {}
    virtual void Render() {}
};

#endif //COMPONENT_H
