#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>
#include <unordered_map>

#include "GameData.h"
#include "Scene.h"
#include "ShopData.h"
#include "TextRenderer.h"
#include "UIManager.h"

class ShopScene : public Scene {
public:
    ShopScene(
        TTF_Font* font,
        GameData* gameData,
        const std::string& shopId =
            "village_shop"
    );

    void Update() override;

    void Render(
        SDL_Renderer* renderer
    ) override;

    void HandleEvents(
        SDL_Event& event
    ) override;

    SceneRequest GetRequest() override;

    void ClearRequest() override;

private:
    void BuildShopUI();

    void AddProductButton(
        const ShopEntry& entry,
        std::size_t index
    );

    void AddCloseButton(int y);

    void BuyItem(
        const std::string& itemId,
        int price
    );

    UIManager uiManager;

    TTF_Font* font = nullptr;
    GameData* gameData = nullptr;

    std::string shopId;
    std::string shopName = "Unknown Shop";

    // 버튼 ID를 실제 상품 정보에 연결한다.
    //
    // 예:
    // shop_item_0
    //   → potion, 50
    //
    // shop_item_1
    //   → old_sword, 100
    std::unordered_map<
        std::string,
        ShopEntry
    > buttonEntries;

    int panelX = 200;
    int panelY = 80;
    int panelWidth = 400;
    int panelHeight = 440;

    SceneRequest request =
        SceneRequest::None;
};