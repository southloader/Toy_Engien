#include "QuestScene.h"
#include "TextRenderer.h"

QuestScene::QuestScene(TTF_Font* font, GameData* gameData) {
    this->font = font;
    this->gameData = gameData;

    UIButton closeButton;
    closeButton.id = "close";
    closeButton.x = 250;
    closeButton.y = 500;
    closeButton.width = 300;
    closeButton.height = 55;
    closeButton.text = "닫기";
    closeButton.normalColor = {180, 70, 70, 255};
    closeButton.hoverColor = {255, 120, 120, 255};
    closeButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(closeButton);    
}

void QuestScene::Update() {
    uiManager.Update();
}

void QuestScene::Render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 160);
    SDL_Rect overlay = {0, 0, 800, 600};
    SDL_RenderFillRect(renderer, &overlay);

    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 240);
    SDL_Rect panel = {120, 70, 560, 500};
    SDL_RenderFillRect(renderer, &panel);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &panel);

    RenderText(renderer, font, "Quest Log", 160, 100);

    const auto& quests = gameData->questLog.GetQuests();

    if (quests.empty()) {
        RenderText(renderer, font, "No quests.", 160, 150);
    }
    else {
        int y = 150;

        for (const auto& quest : quests) {
            RenderText(renderer, font, quest.title, 160, y);
            y += 30;

            RenderText(renderer, font, quest.description, 180, y);
            y += 30;

            std::string progress =
                "Progress: " +
                std::to_string(quest.currentCount) +
                " / " +
                std::to_string(quest.targetCount);

            RenderText(renderer, font, progress, 180, y);
            y += 30;

            std::string reward =
                "Reward: " +
                quest.rewardItemId +
                " x" +
                std::to_string(quest.rewardAmount);

            RenderText(renderer, font, reward, 180, y);
            y += 45;
        }
    }

    uiManager.Render(renderer, font);
}

void QuestScene::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        std::string clickedId =
            uiManager.GetClickedButtonId(mouseX, mouseY);

        if (clickedId == "close") {
            request = SceneRequest::CloseOverlay;
        }
    }

    if (event.type == SDL_KEYDOWN) {
        if (event.key.keysym.sym == SDLK_ESCAPE ||
            event.key.keysym.sym == SDLK_j) {
            request = SceneRequest::CloseOverlay;
        }
    }
}


SceneRequest QuestScene::GetRequest() {
    return request;
}

void QuestScene::ClearRequest() {
    request = SceneRequest::None;
}