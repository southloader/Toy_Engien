#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "Scene.h"
#include "UIManager.h"

enum class MenuAction {
    None,
    StartGame,
    ExitGame
};

class MainMenuScene : public Scene {
public:
    MainMenuScene(TTF_Font* font);

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

    MenuAction GetAction();
    void ClearAction();

private:
    UIManager uiManager;
    TTF_Font* font;
    MenuAction action;
};