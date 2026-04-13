#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Entity.h"


class Game {
public:
    void Init();
    void HandleEvents();
    void Update(Camera * cam);
    void Render(Camera cam);
    void Clean();
    std::vector<std::vector<int>> map;
    int tileSize;

    bool running;

private:
    std::vector<Entity> entities;
    SDL_Window* window;
    SDL_Renderer* renderer;
};