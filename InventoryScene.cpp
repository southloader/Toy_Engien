#include "InventoryScene.h"
#include "TextRenderer.h"

InventoryScene::InventoryScene(TTF_Font* font, GameData* gameData) {
    this->font = font;
    this->gameData = gameData;

    UIButton closeButton;
    closeButton.id = "close";
    closeButton.x = 250;
    closeButton.y = 450;
    closeButton.width = 300;
    closeButton.height = 60;
    closeButton.text = "닫기";
    closeButton.normalColor = {180, 70, 70, 255};
    closeButton.hoverColor = {255, 120, 120, 255};
    closeButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(closeButton);
}

void InventoryScene::Update() {
    uiManager.Update();
}

void InventoryScene::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &overlay);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 240);
    SDL_Rect panel = {180, 100, 440, 420};
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &panel);

    RenderText(renderer, font, "Inventory", 220, 130);

    const auto& slots = gameData->inventory.GetInventory();

    if (slots.empty()) {
        RenderText(renderer, font, "Empty", 220, 180);
    }
    else {
        int y = 180;

        for (const auto& slot : slots) {
            std::string line = "- " + slot.item.name + " x" + std::to_string(slot.count);
            RenderText(renderer, font, line, 220, y);
            y += 32;
        }
    }

    uiManager.Render(renderer, font);
}

void InventoryScene::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        std::string clickedId = uiManager.GetClickedButtonId(mouseX, mouseY);

        if (clickedId == "close") {
            request = SceneRequest::CloseOverlay;
        }
    }

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE ||
            event.key.keysym.sym == SDLK_i) {
            request = SceneRequest::CloseOverlay;
        }
    }
}

SceneRequest InventoryScene::GetRequest() {
    return request;
}

void InventoryScene::ClearRequest() {
    request = SceneRequest::None;
}