#pragma once
#include <stdio.h>
#include <vector>
#include "TextureManager.h"
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "PlayScene.h"
#include "PauseScene.h"

enum class GameState {
    MainMenu,
    Playing,
    ExitConfirm

};

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
    Scene* currentScene;
    SceneManager sceneManager;
    std::vector<Entity> entities;
    std::vector<std::vector<int>> map;
    int tileSize;

    SDL_Window* window;
    SDL_Renderer* renderer;
    TextureManager* textureManager;
    TTF_Font * font;
    Camera camera;
    UIManager uiManager;
    GameState currState;
};