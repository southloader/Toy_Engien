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
    void ShowExitConfirm();
    bool running;

private:
    Scene* currentScene;
    SceneManager sceneManager;
    std::vector<Entity> entities;
    int tileSize;

    SDL_Window* window;
    SDL_Renderer* renderer;
    TextureManager* textureManager;
    TTF_Font * font;
    Camera camera;
    UIManager uiManager;
    GameState currState;
};