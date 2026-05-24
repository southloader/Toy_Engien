#pragma once

#include <SDL.h>
#include <vector>

#include "Scene.h"
#include "Entity.h"
#include "Camera.h"

class PlayScene : public Scene {
public:
    PlayScene();

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

private:
    std::vector<Entity> entities;
    std::vector<std::vector<int>> map;

    Camera camera;

    int tileSize = 50;
    int playerSpeed = 3;

private:
    void InitMap();
    void InitEntities();

    void UpdatePlayer();
    void UpdateCamera(Entity& player);

    bool CheckWallCollision(Entity& entity);
    void RenderMap(SDL_Renderer* renderer);
    void MovePlayer(Entity& player, int moveX, int moveY);
    void MoveAndCollideX(Entity& entity, int moveX);
    void MoveAndCollideY(Entity& entity, int moveY);
};