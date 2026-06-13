#pragma once

#include <SDL.h>
#include <vector>

#include "Camera.h"
#include "TextureManager.h"
#include "Entity.h"

class TileMap {
public:
    TileMap(TextureManager* textureManager);

    void Init();
    void Render(SDL_Renderer* renderer, Camera camera);

    bool CheckCollision(Entity& entity);
    void ResolveCollisionX(Entity& entity, int moveX);
    void ResolveCollisionY(Entity& entity, int moveY);
    
    int GetHeight();
    int GetWidth();

private:
    std::vector<std::vector<int>> map;
    int tileSize = 50;

    TextureManager* textureManager;
};