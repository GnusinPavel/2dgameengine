//
// Created by Pavel Gnusin on 29.09.2020.
//

#include <iostream>
#include "./Constants.h"
#include "./Game.h"
#include "./Map.h"
#include "./Components/TransformComponent.h"
#include "./Components/SpriteComponent.h"
#include "./Components/KeyboardControlComponent.h"
#include "./Components/ColliderComponent.h"
#include "./Components/TextLabelComponent.h"
#include "./Components/ProjectileEmitterComponent.h"
#include "../lib/glm/glm.hpp"

EntityManager manager;
AssetManager *Game::assetManager = new AssetManager(&manager);
SDL_Renderer *Game::renderer;
SDL_Event Game::event;
SDL_Rect Game::camera = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
Map *map;

Game::Game() {
    this->isRunning = false;
}

Game::~Game() = default;

bool Game::IsRunning() const {
    return this->isRunning;
}

void Game::Initialize(int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL." << std::endl;
        return;
    }
    if (TTF_Init() != 0) {
        std::cerr << "Error initializing SDL TTF" << std::endl;
    }

    window = SDL_CreateWindow(
        nullptr,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        SDL_WINDOW_BORDERLESS
    );
    if (!window) {
        std::cerr << "Error creating SDL window." << std::endl;
        return;
    }
    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer) {
        std::cerr << "Error creating SDL renderer." << std::endl;
        return;
    }

    LoadLevel(1);

    isRunning = true;
}

Entity *player;

void Game::LoadLevel(int levelNumber) {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::os, sol::lib::math);

    std::string levelName = "Level" + std::to_string(levelNumber);
    lua.script_file("./assets/scripts/" + levelName + ".lua");

    // ****************************
    // * LOADS ASSETS FROM LUA FILE
    // ****************************
    sol::table levelData = lua[levelName];
    sol::table levelAssets = levelData["assets"];
    unsigned int assetIndex = 0;
    while (true) {
        sol::optional<sol::table> existsAssetIndexMode = levelAssets[assetIndex];
        if (existsAssetIndexMode == sol::nullopt) {
            break;
        } else {
            sol::table asset = levelAssets[assetIndex];
            std::string assetType = asset["type"];
            if (assetType == "texture") {
                std::string assetId = asset["id"];
                std::string assetFile = asset["file"];
                assetManager->AddTexture(assetId, assetFile.c_str());
            } else if (assetType == "font") {
                std::string file = asset["file"];
                int fontSize = asset["fontSize"];
                assetManager->AddFont(asset["id"], file.c_str(), fontSize);
            }
        }
        assetIndex++;
    }

    // ****************************
    // * LOADS MAP FROM LUA FILE
    // ****************************
    sol::table levelMap = levelData["map"];
    std::string mapTextureId = levelMap["textureAssetId"];
    std::string mapFile = levelMap["file"];

    map = new Map(
        mapTextureId,
        static_cast<int>(levelMap["scale"]),
        static_cast<int>(levelMap["tileSize"])
    );
    map->LoadMap(
        mapFile,
        static_cast<int>(levelMap["mapSizeX"]),
        static_cast<int>(levelMap["mapSizeY"])
    );

    // **********************************
    // * LOADS ENTITIES FROM LUA FILE
    // **********************************
    sol::table levelEntities = levelData["entities"];
    unsigned int entityIndex = 0;
    while (true) {
        sol::optional<sol::table> entityTableOpt = levelEntities[entityIndex++];
        if (entityTableOpt == sol::nullopt) {
            break;
        } else {
            sol::table entityTable = entityTableOpt.value();
            std::string name = entityTable["name"];
            LayerType layer = entityTable["layer"];

            Entity &entity(manager.AddEntity(name, layer));

            if (name == "player") {
                player = &entity;
            }

            sol::table componentsTable = entityTable["components"];
            sol::optional<sol::table> transformTableOpt = componentsTable["transform"];
            if (transformTableOpt != sol::nullopt) {
                sol::table transform = transformTableOpt.value();
                int positionX = transform["position"]["x"];
                int positionY = transform["position"]["y"];
                int velocityX = transform["velocity"]["x"];
                int velocityY = transform["velocity"]["y"];
                int width = transform["width"];
                int height = transform["height"];
                int scale = transform["scale"];
                int rotation = transform["rotation"];
                entity.AddComponent<TransformComponent>(positionX, positionY, velocityX, velocityY, width, height,
                                                        scale);
            }
            sol::optional<sol::table> spriteTableOpt = componentsTable["sprite"];
            if (spriteTableOpt != sol::nullopt) {
                sol::table sprite = spriteTableOpt.value();
                std::string textureAssetId = sprite["textureAssetId"];
                bool animated = sprite["animated"].get_or(false);
                int frameCount = sprite["frameCount"].get_or(1);
                int animationSpeed = sprite["animationSpeed"].get_or(1);
                bool hasDirections = sprite["hasDirections"].get_or(false);
                bool fixed = sprite["fixed"].get_or(false);

                std::cout << "Animated " << animated << std::endl;
                std::cout << "frameCount " << frameCount << std::endl;
                std::cout << "animationSpeed " << animationSpeed << std::endl;
                std::cout << "hasDirections " << hasDirections << std::endl;
                std::cout << "fixed " << fixed << std::endl;

                entity.AddComponent<SpriteComponent>(textureAssetId, frameCount, animationSpeed, hasDirections, fixed);
            }
            sol::optional<sol::table> colliderTableOpt = componentsTable["collider"];
            if (colliderTableOpt != sol::nullopt) {
                std::string tag = colliderTableOpt.value()["tag"];
                TransformComponent *transformComponent = entity.GetComponent<TransformComponent>();
                entity.AddComponent<ColliderComponent>(
                    tag, transformComponent->position.x, transformComponent->position.y, transformComponent->width,
                    transformComponent->height);
            }
            sol::optional<sol::table> inputTableOpt = componentsTable["input"];
            if (inputTableOpt != sol::nullopt) {
                sol::table input = inputTableOpt.value();
                std::string up = input["keyboard"]["up"];
                std::string left = input["keyboard"]["left"];
                std::string down = input["keyboard"]["down"];
                std::string right = input["keyboard"]["right"];
                std::string shoot = input["keyboard"]["shoot"];
                entity.AddComponent<KeyboardControlComponent>(up, right, down, left, shoot);
            }
            sol::optional<sol::table> emitterTableOpt = componentsTable["projectileEmitter"];
            if (emitterTableOpt != sol::nullopt) {
                sol::table emitter = emitterTableOpt.value();
                int speed = emitter["speed"];
                int range = emitter["range"];
                int angle = emitter["angle"];
                int width = emitter["width"];
                int height = emitter["height"];
                bool shouldLoop = emitter["shouldLoop"];
                std::string textureAssetId = emitter["textureAssetId"];
                entity.AddComponent<ProjectileEmitterComponent>(speed, angle, range, shouldLoop);
            }
        }
    }

//    assetManager->AddTexture("tank-image", std::string("./assets/images/tank-big-right.png").c_str());
//    assetManager->AddTexture("chopper-image", std::string("./assets/images/chopper-spritesheet.png").c_str());
//    assetManager->AddTexture("radar-image", std::string("./assets/images/radar.png").c_str());
//    assetManager->AddTexture("heliport-image", std::string("./assets/images/heliport.png").c_str());
//    assetManager->AddTexture("jungle-tiletexture", std::string("./assets/tilemaps/jungle.png").c_str());
//    assetManager->AddTexture("projectile-image", std::string("./assets/images/bullet-enemy.png").c_str());
//    assetManager->AddFont("charriot-font", std::string("./assets/fonts/charriot.ttf").c_str(), 14);
//
//    map = new Map("jungle-tiletexture", 2, 32);
//    map->LoadMap("./assets/tilemaps/jungle.map", 25, 20);

//    player.AddComponent<TransformComponent>(240, 106, 0, 0, 32, 32, 1);
//    player.AddComponent<SpriteComponent>("chopper-texture", 2, 90, true, false);
//    player.AddComponent<KeyboardControlComponent>("up", "right", "down", "left", "space");
//    player.AddComponent<ColliderComponent>("PLAYER", 240, 106, 32, 32);

//    Entity &tankEntity(manager.AddEntity("tank", ENEMY_LAYER));
//    tankEntity.AddComponent<TransformComponent>(150, 495, 0, 0, 32, 32, 1);
//    tankEntity.AddComponent<SpriteComponent>("tank-image");
//    tankEntity.AddComponent<ColliderComponent>("ENEMY", 150, 495, 32, 32);
//
//    Entity &projectile(manager.AddEntity("projectile", PROJECTILE_LAYER));
//    projectile.AddComponent<TransformComponent>(150 + 16, 495 + 16, 0, 0, 4, 4, 1);
//    projectile.AddComponent<SpriteComponent>("projectile-image");
//    projectile.AddComponent<ColliderComponent>("PROJECTILE", 150 + 16, 495 + 16, 4, 4);
//    projectile.AddComponent<ProjectileEmitterComponent>(50, 270, 200, true);
//
//    Entity &heliport(manager.AddEntity("Heliport", OBSTACLE_LAYER));
//    heliport.AddComponent<TransformComponent>(470, 420, 0, 0, 32, 32, 1);
//    heliport.AddComponent<SpriteComponent>("heliport-image");
//    heliport.AddComponent<ColliderComponent>("LEVEL_COMPLETE", 470, 420, 32, 32);
//
//    Entity &radarEntity(manager.AddEntity("radar", UI_LAYER));
//    radarEntity.AddComponent<TransformComponent>(720, 15, 0, 0, 64, 64, 1);
//    radarEntity.AddComponent<SpriteComponent>("radar-image", 8, 150, false, true);
//
//    Entity &labelLevelName(manager.AddEntity("LabelLevelName", UI_LAYER));
//    labelLevelName.AddComponent<TextLabelComponent>(10, 10, "First level ...", "charriot-font", WHITE_COLOR);
}

void Game::ProcessInput() {
    SDL_PollEvent(&event);
    switch (event.type) {
        case SDL_QUIT: {
            isRunning = false;
            break;
        }
        case SDL_KEYDOWN: {
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                isRunning = false;
            }
            break;
        }
        default: {
            break;
        }
    }
}

void Game::Update() {

    // Sleep the execution until we reach target frame time in milliseconds
    int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - ticksLastFrame);

    // Only call delay if we are too fast too process this frame
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    // Delta time is the difference in ticks from last frame converted to seconds
    float deltaTime = (SDL_GetTicks() - ticksLastFrame) / 1000.0f;

    // Clamp deltaTIme to a maximum value
    deltaTime = (deltaTime > 0.05f) ? 0.05f : deltaTime;

    // Sets the new ticks for the current frame to be used in the next pass
    ticksLastFrame = SDL_GetTicks();

    manager.Update(deltaTime);

    HandleCameraMovement();
    CheckCollisions();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
    SDL_RenderClear(renderer);

    if (manager.HasNoEntities()) {
        return;
    }

    manager.Render();

    SDL_RenderPresent(renderer);
}

void Game::Destroy() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Game::HandleCameraMovement() {
    TransformComponent *mainPlayerTransform = player->GetComponent<TransformComponent>();
    camera.x = mainPlayerTransform->position.x - (WINDOW_WIDTH / 2);
    camera.y = mainPlayerTransform->position.y - (WINDOW_HEIGHT / 2);

    camera.x = camera.x < 0 ? 0 : camera.x;
    camera.y = camera.y < 0 ? 0 : camera.y;
    camera.x = camera.x > camera.w ? camera.w : camera.x;
    camera.y = camera.y > camera.h ? camera.h : camera.y;
}

void Game::CheckCollisions() {
    CollisionType collisionType = manager.CheckEntityCollisions();
    if (collisionType == PLAYER_ENEMY_COLLISION) {
        ProcessGameOver();
    } else if (collisionType == PLAYER_LEVEL_COMPLETE_COLLISION) {
        ProcessNextLevel();
    } else if (collisionType == PLAYER_PROJECTILE_COLLISION) {
        ProcessGameOver();
    }
}

void Game::ProcessGameOver() {
    std::cout << "Game Over" << std::endl;
    isRunning = false;
}

void Game::ProcessNextLevel() {
    std::cout << "Next Level" << std::endl;
    isRunning = false;
}
