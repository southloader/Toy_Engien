#include "UIButton.h"

void UIButton::Render(SDL_Renderer* renderer, TTF_Font* font) {
    SDL_Rect rect = {x, y, width, height};

    if (isHovered)
        SDL_SetRenderDrawColor(renderer, 100, 100, brightness - 90, 255);
    else
        SDL_SetRenderDrawColor(renderer, 100, 100, brightness, 255);
    SDL_RenderFillRect(renderer, &rect);

    if(font == nullptr || text.empty()) return;

    SDL_Color textColor = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderUTF8_Blended(
        font,
        text.c_str(),
        textColor
    );

    if(surface == nullptr) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);


    if (surface == nullptr) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect textRect = {
        x +(width - surface->w) / 2,
        y + (height - surface->h) / 2,
        surface->w,
        surface->h
    };

    SDL_RenderCopy(renderer, texture, nullptr, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
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
