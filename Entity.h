#pragma once
#include <SDL.h>
#include "Camera.h"

enum EntityType {
    PLAYER,
    ENEMY,
    OBJ,
    NPC
};

class Entity {
public:
    int x, y;
    int width, height;
    EntityType type;

    bool CheckCollision(Entity& other);
    SDL_Texture* texture = nullptr;

    void Update();
    void Render(SDL_Renderer* renderer, Camera camera);
};