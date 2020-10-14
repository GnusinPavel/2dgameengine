//
// Created by Pavel Gnusin on 14.10.2020.
//

#include <fstream>
#include "./Map.h"
#include "./EntityManger.h"
#include "./Components/TileComponent.h"

extern EntityManager manager;

Map::Map(std::string textureId, int scale, int tileSize) {
    this->textureId = textureId;
    this->scale = scale;
    this->tileSize = tileSize;
}

void Map::LoadMap(std::string filePath, int mapSizeX, int mapSizeY) {
    std::fstream mapFile;
    mapFile.open(filePath);

    for (int y = 0; y < mapSizeY; y++) {
        for (int x = 0; x < mapSizeX; x++) {
            char ch;
            mapFile.get(ch);
            int sourceRectY = atoi(&ch) * tileSize;
            mapFile.get(ch);
            int sourceRectX = atoi(&ch) * tileSize;
            AddTile(sourceRectX, sourceRectY, x * (scale * tileSize), y * (scale * tileSize));
            mapFile.ignore();
        }
    }
    mapFile.close();
}

void Map::AddTile(int sourceX, int sourceY, int x, int y) {
    Entity &newTile(manager.AddEntity("Tile"));
    newTile.AddComponent<TileComponent>(sourceX, sourceY, x, y, tileSize, scale, textureId);
}