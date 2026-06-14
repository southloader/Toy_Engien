#pragma once
#include "Camera.h"
#include "Animator.h"

enum EntityType {
    PLAYER,
    ENEMY,
    OBJ,
    NPC
};

enum class Direction {
    Left,
    Right
};

class Entity {
public:
    int x, y;
    int width, height;
    EntityType type;
    Direction direction = Direction::Right;

    bool CheckCollision(Entity& other);
    SDL_Texture* texture = nullptr;
    Animator* animator = nullptr;

    void Render(SDL_Renderer* renderer, Camera camera);
};