#pragma once
#include <SDL.h>
#include "Camera.h"

enum EntityType {
    PLAYER,
    ENEMY,
    OBJ
};

class Entity {
public:
    int x, y;
    int width, height;
    EntityType type;

    bool CheckCollision(Entity& other);

    void Update();
    void Render(SDL_Renderer* renderer, Camera camera);
};