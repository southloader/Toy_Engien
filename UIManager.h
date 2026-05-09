#pragma once
#include <vector>
#include "UIButton.h"

class UIManager {
public:
    std::vector<UIButton> buttons;

    std::string GetClickedButtonId(int mouseX, int mouseY);

    void Update();
    void Render(SDL_Renderer* renderer, TTF_Font* font);

    void AddButton(UIButton button);

};