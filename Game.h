#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Entity.h"


class Game {
public:
    void Init();
    void HandleEvents();
    void Update();
    void Render();
    void Clean();

    void ShowExitConfirm();
    bool running;

private:
    std::vector<Entity> entities;
    std::vector<std::vector<int>> map;
    int tileSize;

    SDL_Window* window;
    SDL_Renderer* renderer;

    Camera camera;
};