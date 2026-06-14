#pragma once

#include <SDL.h>
#include <vector>

class Animation {
public:
    void AddFrame(SDL_Texture* texture);
    void Update();
    void SetFrameDelay(Uint32 delay);
    SDL_Texture* GetCurrentFrame();

private:
    std::vector<SDL_Texture*> frames;

    int currentFrame = 0;

    Uint32 lastFrameTime = 0;
    Uint32 frameDelay = 150;
};