#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class QuestNotification {
public:
    void Show(const std::string& message);
    void Update();
    void Render(SDL_Renderer* renderer, TTF_Font* font);

private:
    bool visible = false;
    std::string message;

    Uint32 startTime = 0;
    Uint32 duration = 2500;
};