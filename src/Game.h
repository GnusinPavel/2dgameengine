//
// Created by Pavel Gnusin on 28.09.2020.
//

#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

class AssetManager;

class Game {
private:
    bool isRunning = false;
    SDL_Window *window;

public:
    Game();
    ~Game();
    int ticksLastFrame = 0;
    bool IsRunning() const;
    void Initialize(int width, int height);
    static SDL_Renderer *renderer;
    static AssetManager* assetManager;
    static SDL_Event event;
    void LoadLevel(int levelNumber);
    void ProcessInput();
    void Update();
    void Render();
    void Destroy();
};

#endif //GAME_H
