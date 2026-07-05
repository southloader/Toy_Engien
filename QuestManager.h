#pragma once 

#include <string>
#include <unordered_map>

#include "Quest.h"

class GameData;

class QuestManager {
public:
    QuestManager(GameData* gameData);
    Quest* GetQuest(const std::string& id);

    void RegisterQuest(const Quest& quest);
    void RegisterAllFromDatabase();
    bool AcceptQuest(const std::string& id);
    bool AbandonQuest(const std::string& id);
    void UpdateQuestProgress();
    bool CanComplete(const std::string& id);
    QuestResult CompleteQuest(const std::string& id);
    bool GiveReward(const std::string& id);

    bool IsActive(const std::string& id);
    bool IsCompleted(const std::string& id);  

private:
    GameData* gameData;

    std::unordered_map<std::string, Quest> questDatabase;
};