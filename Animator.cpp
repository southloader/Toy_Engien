#include "Animator.h"

void Animator::AddAnimation(const std::string& name, Animation animation) {
    animations[name] = animation;
}

void Animator::Play(const std::string& name) {
    if (currentAnimation == name) return;

    if (animations.find(name) == animations.end()) return;

    currentAnimation = name;
}

void Animator::Update() {
    if (currentAnimation == "") return;

    animations[currentAnimation].Update();
}

SDL_Texture* Animator::GetCurrentFrame() {
    if (currentAnimation == "") return nullptr;

    return animations[currentAnimation].GetCurrentFrame();
}