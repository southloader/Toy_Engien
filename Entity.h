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
    std::string characterId;
    std::string instanceId;
    int x = 0;
    int y = 0;

    int width = 0;
    int height = 0;

    EntityType type = OBJ;
    Direction direction = Direction::Right;

    SDL_Texture* texture = nullptr;
    Animator animator;

    bool CheckCollision(Entity& other);

    void LoadCharacter(
        const CharacterData& data,
        TextureManager* textureManager
    );

    void Render(
        SDL_Renderer* renderer,
        Camera camera
    );
};