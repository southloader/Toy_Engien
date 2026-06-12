#include "PauseScene.h"

PauseScene::PauseScene(TTF_Font* font) {

    this->font = font;

    request = SceneRequest::None;

    UIButton resumeButton;
    resumeButton.id = "resume";
    resumeButton.x = 300;
    resumeButton.y = 150;
    resumeButton.width = 200;
    resumeButton.height = 80;
    resumeButton.text = "계속하기";
    resumeButton.normalColor = {70, 70, 180, 255};
    resumeButton.hoverColor = {120, 120, 255, 255};
    resumeButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(resumeButton);

    UIButton menuButton;
    menuButton.id = "menu";
    menuButton.x = 300;
    menuButton.y = 260;
    menuButton.width = 200;
    menuButton.height = 80;
    menuButton.text = "메인 메뉴";
    menuButton.normalColor = {70, 180, 70, 255};
    menuButton.hoverColor = {120, 255, 120, 255};
    menuButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(menuButton);

    UIButton exitButton;
    exitButton.id = "exit";
    exitButton.x = 300;
    exitButton.y = 370;
    exitButton.width = 200;
    exitButton.height = 80;
    exitButton.text = "게임 종료";
    exitButton.normalColor = {180, 70, 70, 255};
    exitButton.hoverColor = {255, 120, 120, 255};
    exitButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(exitButton);
}

void PauseScene::Update() {
    uiManager.Update();
}

void PauseScene::Render(SDL_Renderer* renderer) {

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 80);

    SDL_Rect overlay = {0, 0, 800, 600};

    SDL_RenderFillRect(renderer, &overlay);

    uiManager.Render(renderer, font);
}

void PauseScene::HandleEvents(SDL_Event& event) {

    if (event.type == SDL_MOUSEBUTTONDOWN) {

        int mouseX = event.button.x;
        int mouseY = event.button.y;

        std::string clickedId =
            uiManager.GetClickedButtonId(mouseX, mouseY);

        if (clickedId == "resume") {
            request = SceneRequest::GoToPlay;
        }

        else if (clickedId == "menu") {
            request = SceneRequest::GoToMainMenu;
        }

        else if (clickedId == "exit") {
            request = SceneRequest::ExitGame;
        }
    }
}

SceneRequest PauseScene::GetRequest() {
    return request;
}

void PauseScene::ClearRequest() {
    request = SceneRequest::None;
}