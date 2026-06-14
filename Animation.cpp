#include "Animation.h"

void Animation::AddFrame(SDL_Texture* texture){
    frames.push_back(texture);
}

void Animation::Update(){
    if (frames.empty()) return;

    Uint32 currentTime = SDL_GetTicks();

    if (currentTime - lastFrameTime >= frameDelay) {
        currentFrame++;
        if (currentFrame >= frames.size()) {
            currentFrame = 0;
        }

        lastFrameTime = currentTime;
    }
}

SDL_Texture* Animation::GetCurrentFrame() {
    if (frames.empty()) return nullptr;

    return frames[currentFrame];
}

void Animation::SetFrameDelay(Uint32 delay){
    frameDelay = delay;
}