//
// Created by Pavel Gnusin on 07.10.2020.
//

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include "./EntityManger.h"
#include "./TextureManager.h"

class AssetManager {
private:
    EntityManager* manager;
    std::map<std::string, SDL_Texture*> textures;
public:
    AssetManager(EntityManager* manager);
    ~AssetManager();
    void ClearData();
    void AddTexture(std::string textureId, const char* filePath);
    SDL_Texture* GetTexture(std::string textureId);
};


#endif //ASSETMANAGER_H
