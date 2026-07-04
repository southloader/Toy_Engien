#include "QuestNotification.h"
#include "TextRenderer.h"

void QuestNotification::Show(const std::string& message) {
    this->message = message;
    visible = true;
    startTime = SDL_GetTicks();
}

void QuestNotification::Update() {
    if (!visible) return;

    Uint32 now = SDL_GetTicks();

    if(now - startTime >= duration) {
        visible = false;
    }
}

void QuestNotification::Render(SDL_Renderer* renderer, TTF_Font* font) {
    if (!visible) return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Rect box = {500, 40, 260, 70};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
    SDL_RenderFillRect(renderer, &box);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &box);

    RenderText(renderer, font, message, 520, 65);
}