#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Scene.h"
#include "UIManager.h"
#include "GameData.h"

class QuestScene : public Scene {
public:
    QuestScene(TTF_Font* font, GameData* gameData);

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

    SceneRequest GetRequest() override;
    void ClearRequest() override;

private:
    UIManager uiManager;
    TTF_Font* font;
    GameData* gameData;

    SceneRequest request = SceneRequest::None;
};