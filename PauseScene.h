#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Scene.h"
#include "UIManager.h"

enum class PauseAction {
    None,
    Resume,
    MainMenu,
    Exit
};

class PauseScene : public Scene {
public:
    PauseScene(TTF_Font* font);

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

    PauseAction GetAction();
    void ClearAction();

private:
    UIManager uiManager;

    TTF_Font* font;

    PauseAction action;
};