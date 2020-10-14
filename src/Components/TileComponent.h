//
// Created by Pavel Gnusin on 14.10.2020.
//

#ifndef TILECOMPONENT_H
#define TILECOMPONENT_H


#include <SDL2/SDL.h>
#include "../Component.h"
#include "../EntityManger.h"
#include "../../lib/glm/glm.hpp"
#include "../TextureManager.h"

class TileComponent : public Component {
public:
    SDL_Texture *texture;
    SDL_Rect sourceRectangle;
    SDL_Rect destinationRectangle;
    glm::vec2 position;

    TileComponent(int sourceRectangleX, int sourceRectangleY, int x, int y, int tileSize, int tileScale,
                  std::string assetTextureId) {
        texture = Game::assetManager->GetTexture(assetTextureId);

        sourceRectangle.x = sourceRectangleX;
        sourceRectangle.y = sourceRectangleY;
        sourceRectangle.w = tileSize;
        sourceRectangle.h = tileSize;

        destinationRectangle.x = x;
        destinationRectangle.y = y;
        destinationRectangle.w = tileSize * tileScale;
        destinationRectangle.h = tileSize * tileScale;

        position.x = x;
        position.y = y;
    }

    ~TileComponent() {
        SDL_DestroyTexture(texture);
    }

    void Update(float deltaTime) override {
        // TODO: here is where we will take care of the tile positions
        // based on the camera control that will kep moving around

    }

    void Render() override {
        TextureManager::Draw(texture, sourceRectangle, destinationRectangle, SDL_FLIP_NONE);
    }
};


#endif //TILECOMPONENT_H