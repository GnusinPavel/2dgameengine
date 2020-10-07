//
// Created by Pavel Gnusin on 07.10.2020.
//

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include "./Game.h"

class TextureManager {
public:
    static SDL_Texture* LoadTexture(const char* filename);
    static void Draw(SDL_Texture* texture, SDL_Rect sourceRectangle, SDL_Rect destinationRectangle, SDL_RendererFlip flip);
};

#endif //TEXTUREMANAGER_H
