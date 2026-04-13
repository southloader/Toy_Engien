#pragma once
#include <SDL2/SDL.h>

struct Camera {
    int x, y;
};

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