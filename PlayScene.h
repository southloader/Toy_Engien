#pragma once

#include <SDL.h>
#include <vector>

#include "TileMap.h"
#include "TextureManager.h"
#include "Scene.h"
#include "Entity.h"
#include "Camera.h"
#include "NPC.h"
#include "DialogueBox.h"

class PlayScene : public Scene {
public:
    PlayScene(TextureManager* textureManager, TTF_Font* font);
    ~PlayScene();

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;

    bool IsNear(Entity& a, Entity& b, int distance);
    SceneRequest GetRequest() override;
    void ClearRequest() override;
    Entity* GetPlayer();

private:
    std::vector<Entity> entities;
    TextureManager* textureManager;
    TileMap* tileMap;
    SceneRequest request;
    Camera camera;
    std::vector<NPC> npcs;
    DialogueBox dialogueBox;
    TTF_Font* font;
    
    int playerSpeed = 3;

private:
    void InitEntities();
    void InitNPCs();
    
    void UpdatePlayer();
    void UpdateCamera(Entity& player);
    void CheckNPCInteraction();

    void MovePlayer(Entity& player, int moveX, int moveY);
    void MoveAndCollideX(Entity& entity, int moveX);
    void MoveAndCollideY(Entity& entity, int moveY);
};