#include "TileMap.h"

TileMap::TileMap(TextureManager* textureManager){
    this->textureManager = textureManager;
}

void TileMap::Init() {
    map = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
        {1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };
}

void TileMap::Render(SDL_Renderer* renderer, Camera camera) {
    SDL_Texture* wallTexture = textureManager->GetTexture("wall");

    for (int y = 0; y < map.size(); y++){
        for (int x = 0; x < map[y].size(); x++){
            if(map[y][x] != 1) continue;

            SDL_Rect wall = {
                x * tileSize - camera.x,
                y * tileSize - camera.y,
                tileSize,
                tileSize
            };

            if (wallTexture != nullptr) {
                SDL_RenderCopy(renderer, wallTexture, nullptr, &wall);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                SDL_RenderFillRect(renderer, &wall);
            }
        }
    }
}

bool TileMap::CheckCollision(Entity& entity){
    SDL_Rect entityRect = {
        entity.x,
        entity.y,
        entity.width,
        entity.height
    };

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 1) continue;

            SDL_Rect wallRect = {
                x * tileSize,
                y * tileSize,
                tileSize,
                tileSize
            };

            if (SDL_HasIntersection(&entityRect, &wallRect)) {
                return true;
            }
        }
    }

    return false;
}

void TileMap::ResolveCollisionX(Entity& entity, int moveX) {
    if (moveX == 0) return;

    SDL_Rect entityRect = {
        entity.x,
        entity.y,
        entity.width,
        entity.height
    };

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 1) continue;

            SDL_Rect wallRect = {
                x * tileSize,
                y * tileSize,
                tileSize,
                tileSize
            };

            if (SDL_HasIntersection(&entityRect, &wallRect)) {
                if (moveX > 0) {
                    entity.x = wallRect.x - entity.width;
                }
                else if (moveX < 0) {
                    entity.x = wallRect.x + wallRect.w;
                }

                entityRect.x = entity.x;
            }
        }
    }
}

void TileMap::ResolveCollisionY(Entity& entity, int moveY) {
    if (moveY == 0) return;

    SDL_Rect entityRect = {
        entity.x,
        entity.y,
        entity.width,
        entity.height
    };

    for (int y = 0; y < map.size(); y++) {
        for (int x = 0; x < map[y].size(); x++) {
            if (map[y][x] != 1) continue;

            SDL_Rect wallRect = {
                x * tileSize,
                y * tileSize,
                tileSize,
                tileSize
            };

            if (SDL_HasIntersection(&entityRect, &wallRect)) {
                if (moveY > 0) {
                    entity.y = wallRect.y - entity.height;
                }
                else if (moveY < 0) {
                    entity.y = wallRect.y + wallRect.h;
                }

                entityRect.y = entity.y;
            }
        }
    }
}

int TileMap::GetWidth() {
    if (map.empty()) return 0;
    return map[0].size() * tileSize;
}

int TileMap::GetHeight() {
    return map.size() * tileSize;
}