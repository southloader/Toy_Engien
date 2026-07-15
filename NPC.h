#pragma once

#include <string>
#include <vector>
#include <cmath>

#include "Entity.h"
#include "Camera.h"
#include "TextureManager.h"
#include "Animator.h"

enum class NPCBehavior {
    Idle,
    Patrol,
    FollowPlayer
};

struct QuestInteraction {
    std::string questId;

    bool canOffer = false;
    bool canComplete = false;
    bool canAbandon = false;
};


class NPC {
public:
    NPC();

    void SetName(const std::string& name);
    void SetPosition(int x, int y);
    void SetSize(int width, int height);
    void SetTexture(SDL_Texture* texture);
    void SetDialogue(const std::vector<std::string>& lines);
    void SetBehavior(NPCBehavior behavior);
    void SetPatrolPoints(const std::vector<SDL_Point>& points);
    void SetMoveSpeed(int speed);

    void AddQuestInteraction(const QuestInteraction& interaction);
    const std::vector<QuestInteraction>& GetQuestInteractions() const;
    bool HasQuestInteractions() const;

    void Update(Entity* player);
    void Render(SDL_Renderer* renderer, Camera camera);

    Entity& GetEntity();

    std::string GetName();
    std::vector<std::string> GetDialogue();

private:
    std::string name;
    std::vector<std::string> dialogueLines;

    NPCBehavior behavior = NPCBehavior::Idle;
    std::vector<QuestInteraction> questInteractions;

    std::vector<SDL_Point> patrolPoints;
    int currentPatrolIndex = 0;
    int moveSpeed = 2;

    Entity entity;
};