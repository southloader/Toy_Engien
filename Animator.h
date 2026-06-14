#pragma once

#include <string>
#include <unordered_map>

#include "Animation.h"

class Animator {
public:
    void AddAnimation(const std::string& name, Animation animation);
    void Play(const std::string& name);
    void Update();
    SDL_Texture* GetCurrentFrame();

private:
    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimation = "";
};