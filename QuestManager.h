#pragma once 

#include <string>
#include <unordered_map>

#include "Quest.h"
#include "EventManager.h"

class GameData;

class EventManager;

class QuestManager {
public:
    QuestManager(GameData* gameData, EventManager* eventManager);
    ~QuestManager();

    Quest* GetQuest(const std::string& id);
    const Quest* GetQuestDefinition(const std::string& id) const;

    void RegisterQuest(const Quest& quest);
    void RegisterAllFromDatabase();
    
    bool AcceptQuest(const std::string& id);
    bool AbandonQuest(const std::string& id);
    
    void UpdateQuestProgress();
    void UpdateCollectQuestProgress(const std::string& itemId);
    void UpdateKillQuestProgress(const std::string& enemyId, int killCount = 1);
    bool CanComplete(const std::string& id);
    QuestResult CompleteQuest(const std::string& id);
    bool GiveReward(const std::string& id);

    bool IsActive(const std::string& id);
    bool IsCompleted(const std::string& id);  

private:
    GameData* gameData;
    EventManager* eventManager;

    std::unordered_map<std::string, Quest> questDatabase;

    EventManager::ListenerId itemAddedListenerId = 0;
    EventManager::ListenerId itemRemovedListenerId = 0;
    EventManager::ListenerId enemyKilledListenerId = 0;
};