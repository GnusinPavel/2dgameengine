//
// Created by Pavel Gnusin on 04.10.2020.
//

#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include "../EntityManger.h"
#include "../../lib/glm/glm.hpp"
#include "../Game.h"
#include "../Constants.h"

class TransformComponent: public Component {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    int width;
    int height;
    int scale;

    TransformComponent(int posX, int posY, int velX, int velY, int w, int h, int s) {
        position = glm::vec2(posX, posY);
        velocity = glm::vec2(velX, velY);
        width = w;
        height = h;
        scale = s;
    }

    void Initialize() override {

    }

    void Update(float deltaTime) override {
        // TODO:
        position.x += velocity.x * deltaTime;
        if (position.x < 0) {
            position.x = 0;
        } else if (int(position.x) > WINDOW_WIDTH - width * scale) {
            position.x = WINDOW_WIDTH - width * scale;
        }
        position.y += velocity.y * deltaTime;
        if (position.y < 0) {
            position.y = 0;
        } else if (int(position.y) > WINDOW_HEIGHT - height * scale) {
            position.y = WINDOW_HEIGHT - height * scale;
        }
    }

    void Render() override {

    }
};


#endif //TRANSFORMCOMPONENT_H
