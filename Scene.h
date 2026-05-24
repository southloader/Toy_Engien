#pragma once
#include <SDL.h>

class Scene {
public:
    virtual void Update() = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
    virtual void HandleEvents(SDL_Event& event) = 0;

    virtual ~Scene() {}
};