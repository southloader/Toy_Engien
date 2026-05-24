#include "MainMenuScene.h"

MainMenuScene::MainMenuScene(TTF_Font* font) {
    this->font = font;
    action = MenuAction::None;

    UIButton startButton;
    startButton.id = "start";
    startButton.x = 300;
    startButton.y = 190;
    startButton.width = 200;
    startButton.height = 80;
    startButton.text = "게임 시작";
    startButton.normalColor = {0, 180, 0, 255};
    startButton.hoverColor = {0, 150, 0, 255};
    startButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(startButton);

    UIButton exitButton;
    exitButton.id = "exit";
    exitButton.x = 300;
    exitButton.y = 300;
    exitButton.width = 200;
    exitButton.height = 80;
    exitButton.text = "종료";
    exitButton.normalColor = {180, 0, 0, 255};
    exitButton.hoverColor = {150, 0, 0, 255};
    exitButton.textColor = {255, 255, 255, 255};

    uiManager.AddButton(exitButton);
}

void MainMenuScene::Update() {
    uiManager.Update();
}

void MainMenuScene::Render(SDL_Renderer* renderer) {
    uiManager.Render(renderer, font);
}

void MainMenuScene::HandleEvents(SDL_Event& event) {
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int mouseX = event.button.x;
        int mouseY = event.button.y;

        std::string clickedId = uiManager.GetClickedButtonId(mouseX, mouseY);

        if (clickedId == "start") {
            action = MenuAction::StartGame;
        }
        else if (clickedId == "exit") {
            action = MenuAction::ExitGame;
        }
    }
}

MenuAction MainMenuScene::GetAction() {
    return action;
}

void MainMenuScene::ClearAction() {
    action = MenuAction::None;
}