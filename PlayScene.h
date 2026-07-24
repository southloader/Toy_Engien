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
#include "GameData.h"
#include "QuestManager.h"
#include "QuestNotification.h"

#include "SampleCombatSession.h"

class SaveManager;

class EventManager;

class PlayScene : public Scene {
public:
    PlayScene(
        TextureManager* textureManager, 
        TTF_Font* font, GameData* gameData, 
        QuestManager* questManager, 
        SaveManager* saveManager, 
        EventManager* eventManager, 
        SampleCombatSession* sampleCombatSession
    );
    ~PlayScene();

    void Update() override;
    void Render(SDL_Renderer* renderer) override;
    void HandleEvents(SDL_Event& event) override;
    void ProcessDialogueAction();

    bool IsNear(Entity& a, Entity& b, int distance);
    SceneRequest GetRequest() override;
    void ClearRequest() override;
    Entity* GetPlayer();

private:
    std::vector<Entity> entities;
    std::vector<NPC> npcs;
    TextureManager* textureManager;
    TileMap* tileMap;
    SceneRequest request;
    Camera camera;
    DialogueBox dialogueBox;
    TTF_Font* font;
    GameData* gameData;
    QuestManager* questManager;
    QuestNotification questNotification;
    SaveManager* saveManager;
    EventManager* eventManager = nullptr;
    SampleCombatSession* sampleCombatSession = nullptr;

    unsigned int questAcceptedListenerId = 0;
    unsigned int questCompletedListenerId = 0;
    unsigned int questAbandonedListenerId = 0;
    unsigned int gameSavedListenerId = 0;
    unsigned int gameLoadedListenerId = 0;
    
    int playerSpeed = 3;

private:
    void InitEntities();
    void InitNPCs();
    
    void UpdatePlayer();
    void UpdateEntities();
    void UpdateCamera(Entity& player);
    void CheckNPCInteraction();


    bool CheckEnemyInteraction();
    void StartCombatWithEnemy(const Entity& enemy);
    void ProcessCombatResult();
    bool RemoveEntityByInstanceId(const std::string& instanceId);

    void ShowQuestInteraction(NPC& npc);
    
    void MovePlayer(Entity& player, int moveX, int moveY);
    void MoveAndCollideX(Entity& entity, int moveX);
    void MoveAndCollideY(Entity& entity, int moveY);
};