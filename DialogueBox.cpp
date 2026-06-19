#include "DialogueBox.h"

void DialogueBox::Show(const std::string& speaker, const std::vector<std::string>& lines) {
    speakerName = speaker;
    dialogueLines = lines;
    currentLine = 0;
    visible = true;
}

void DialogueBox::Hide() {
    visible = false;
}

void DialogueBox::Next() {
    if (!visible) return;

    currentLine++;

    if (currentLine >= dialogueLines.size()) {
        Hide();
    }
}

bool DialogueBox::IsVisible() {
    return visible;
}

void RenderText(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y) {
    SDL_Color color = {255, 255, 255, 255};

    SDL_Surface* surface = TTF_RenderUTF8_Blended(font, text.c_str(), color);
    if (!surface) return;

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dst = {x, y, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, nullptr, &dst);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void DialogueBox::Render(SDL_Renderer* renderer, TTF_Font* font) {
    if (!visible) return;
    if (font == nullptr) return;
    if (dialogueLines.empty()) return;

    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    SDL_Rect box = {50, 420, 700, 140};

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 210);
    SDL_RenderFillRect(renderer, &box);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &box);

    RenderText(renderer, font, speakerName, 80, 440);
    RenderText(renderer, font, dialogueLines[currentLine], 80, 485);
}