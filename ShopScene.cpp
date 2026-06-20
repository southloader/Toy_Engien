#include "ShopScene.h"

ShopScene::ShopScene(TTF_Font* font, GameData* gameData) {
    this->font = font;
    this->gameData = gameData;

    UIButton potionButton;
    potionButton.id = "potion";
    potionButton.x = 250;
    potionButton.y = 180;
    potionButton.width = 300;
    potionButton.height = 60;
    potionButton.text = "포션 - 50G";
    potionButton.normalColor = {70, 70, 180, 255};
    potionButton.hoverColor = {120, 120, 255, 255};
    potionButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(potionButton);

    UIButton swordButton;
    swordButton.id = "sword";
    swordButton.x = 250;
    swordButton.y = 260;
    swordButton.width = 300;
    swordButton.height = 60;
    swordButton.text = "낡은 검 - 100G";
    swordButton.normalColor = {70, 70, 180, 255};
    swordButton.hoverColor = {120, 120, 255, 255};
    swordButton.textColor = {255, 255, 255, 255};
        
    uiManager.AddButton(swordButton);

    UIButton closeButton;
    closeButton.id = "close";
    closeButton.x = 250;
    closeButton.y = 360;
    closeButton.width = 300;
    closeButton.height = 60;
    closeButton.text = "닫기";
    closeButton.normalColor = {180, 70, 70, 255};
    closeButton.hoverColor = {255, 120, 120, 255};
    closeButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(closeButton);
}

void ShopScene::Update(){
    uiManager.Update();
}

void ShopScene::Render(SDL_Renderer* renderer){
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &overlay);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 240);
    SDL_Rect panel = {200, 120, 400, 360};
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &panel);

    std::string goldText = "보유 골드: " + std::to_string(gameData->gold) + "G";
    RenderText(renderer, font, goldText, 250, 140);

    uiManager.Render(renderer, font);  
}

void ShopScene::BuyItem(const std::string& itemName, int price){
    if (gameData->gold >= price) {
        gameData->gold -= price;
        gameData->inventory.AddItem(itemName);

        printf("Bought %s for %dG\n",itemName.c_str(), price);
        printf("Gold left: %d\n", gameData->gold);
    }
    else {
        printf("Not enough gold\n");
    }
}

void ShopScene::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        std::string clickedId = uiManager.GetClickedButtonId(mouseX, mouseY);
        
        if (clickedId == "potion") {
            BuyItem("Potion", 50);
        }
        else if (clickedId == "sword") {
            BuyItem("Old Sword", 100);
        }
        else if (clickedId == "close") {
            request = SceneRequest::CloseOverlay;
        }
    }

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE) {
            request = SceneRequest::CloseOverlay;
        }
    }
}

SceneRequest ShopScene::GetRequest() {
    return request;
}

void ShopScene::ClearRequest() {
    request = SceneRequest::None;
}