#include "UIButton.h"

void UIButton::Render(SDL_Renderer* renderer) {
    SDL_Rect rect = {x, y, width, height};

    if (isHovered)
        SDL_SetRenderDrawColor(renderer, 100, 100, brightness - 90, 255);
    else
        SDL_SetRenderDrawColor(renderer, 100, 100, brightness, 255);
    SDL_RenderFillRect(renderer, &rect);

}

bool UIButton::IsClicked(int mouseX, int mouseY) {
    return(
        mouseX >= x &&
        mouseX <= x + width &&
        mouseY >= y &&
        mouseY <= y + height
    );
}

void UIButton::Update() {

    int mouseX, mouseY;

    SDL_GetMouseState(&mouseX, &mouseY);

    isHovered = IsClicked(mouseX, mouseY);
}
