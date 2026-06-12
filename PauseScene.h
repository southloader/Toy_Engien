#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Scene.h"
#include "UIManager.h"

class PauseScene : public Scene {
public:
    PauseScene(TTF_Font* font);

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

    SceneRequest GetRequest() override;
    void ClearRequest() override;

private:
    UIManager uiManager;
    SceneRequest request;
    TTF_Font* font;
};