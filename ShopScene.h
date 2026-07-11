#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include "Scene.h"
#include "UIManager.h"
#include "GameData.h"
#include "TextRenderer.h"
#include "ItemDatabase.h"

class ShopScene : public Scene {
public:
    ShopScene(TTF_Font* font, GameData* gameData);

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

    SceneRequest GetRequest() override;
    void ClearRequest() override;
    void BuyItem(const std::string& itemId,int price);

private:
    UIManager uiManager;
    TTF_Font* font;
    GameData* gameData;

    SceneRequest request = SceneRequest::None;
};