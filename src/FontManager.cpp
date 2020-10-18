//
// Created by Pavel Gnusin on 17.10.2020.
//

#include "FontManager.h"

TTF_Font *FontManager::LoadFont(const char *fileName, int fontSize) {
    return TTF_OpenFont(fileName, fontSize);
}

void FontManager::Draw(SDL_Texture *texture, SDL_Rect position) {
    SDL_RenderCopy(Game::renderer, texture, nullptr, &position);
}