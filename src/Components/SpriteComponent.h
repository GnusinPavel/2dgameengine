//
// Created by Pavel Gnusin on 09.10.2020.
//

#ifndef SPRITECOMPONENT_H
#define SPRITECOMPONENT_H


#include <SDL2/SDL_render.h>
#include "../Component.h"
#include "../AssetManager.h"
#include "TransformComponent.h"

class SpriteComponent: public Component {
private:
    TransformComponent* transform;
    SDL_Texture* texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;

public:
    SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

    SpriteComponent(const char* filePath) {
        SetTexture(filePath);
    }

    void SetTexture(std::string assetTextureId) {
        texture = Game::assetManager->GetTexture(assetTextureId);
    }

    void Initialize() override {
        transform = owner->GetComponent<TransformComponent>();
        sourceRectangle.x = 0;
        sourceRectangle.y = 0;
        sourceRectangle.w = transform->width;
        sourceRectangle.h = transform->height;
    }

    void Update(float deltaTime) override {
        destinationRectangle.x = transform->position.x;
        destinationRectangle.y = transform->position.y;
        destinationRectangle.w = transform->width * transform->scale;
        destinationRectangle.h = transform->height * transform->scale;
    }

    void Render() override {
        TextureManager::Draw(texture, sourceRectangle, destinationRectangle,
                             spriteFlip);
    }
};


#endif //SPRITECOMPONENT_H
