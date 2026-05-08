#pragma once
#include <stdio.h>
#include <vector>
#include <SDL.h>
#include "Entity.h"
#include "UIButton.h"
#include <SDL_ttf.h>

class Game {
public:
    void Init();
    void HandleEvents();
    void Update();
    void Render();
    void Clean();

    void RenderText(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y);
    void ShowExitConfirm();
    bool running;

private:
    std::vector<Entity> entities;
    std::vector<std::vector<int>> map;
    int tileSize;

    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font * font;
    Camera camera;

    UIButton exitButton;
};