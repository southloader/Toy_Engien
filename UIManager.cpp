#include "UIManager.h"

void UIManager::AddButton(UIButton button) {
    buttons.push_back(button);
}

void UIManager::Update(){
    for (auto & button : buttons) {
        button.Update();
    }
}

void UIManager::Render(SDL_Renderer* renderer, TTF_Font* font) {
    for (auto & button : buttons){
        button.Render(renderer, font);
    }
}

std::string UIManager::GetClickedButtonId(int mouseX, int mouseY){
    for (auto& button : buttons){
        if (button.IsClicked(mouseX, mouseY)){
            return button.id;
        }
    }

    return "";
};
