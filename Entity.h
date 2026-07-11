#pragma once

#include "Camera.h"
#include "Animator.h"
#include "CharacterData.h"
#include "CombatStats.h"
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
    int x = 0;
    int y = 0;

    int width = 0;
    int height = 0;

    EntityType type = OBJ;
    Direction direction = Direction::Right;

    SDL_Texture* texture = nullptr;
    Animator animator;

    // 해당 Entity만의 현재 전투 상태
    CombatStats combatStats;

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