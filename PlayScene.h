#pragma once

#include <SDL.h>
#include <vector>

#include "TileMap.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Camera.h"

class PlayScene : public Scene {
public:
    PlayScene(TextureManager* textureManager);
    ~PlayScene();

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

    SceneRequest GetRequest() override;
    void ClearRequest() override;

private:
    std::vector<Entity> entities;
    
    TextureManager* textureManager;
    TileMap* tileMap;
    
    SceneRequest request;
    Camera camera;

    int playerSpeed = 3;

private:
    void InitEntities();

    void UpdatePlayer();
    void UpdateCamera(Entity& player);

    void MovePlayer(Entity& player, int moveX, int moveY);
    void MoveAndCollideX(Entity& entity, int moveX);
    void MoveAndCollideY(Entity& entity, int moveY);
};