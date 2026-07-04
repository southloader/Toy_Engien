#pragma once
#include "Camera.h"
#include "Animator.h"
#include "CharacterData.h"
#include "TextureManager.h"

enum EntityType {
    PLAYER,
    ENEMY,
    OBJ,
    NPC_TYPE
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
    void LoadCharacter(const CharacterData& data, TextureManager* textureManager);
    SDL_Texture* texture = nullptr;
    Animator animator;

    void Render(SDL_Renderer* renderer, Camera camera);
};