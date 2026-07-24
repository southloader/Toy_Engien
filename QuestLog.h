#pragma once
#include <vector>
#include "Quest.h"

class QuestLog {
public:
    void Clear(); 
    void AddQuest(const Quest& quest);
    bool HasQuest(const std::string& id) const;
    bool RemoveQuest(const std::string& id);
    Quest* GetQuest(const std::string& id);
    const Quest* GetQuest(const std::string& id) const;
    std::vector<Quest>& GetMutableQuests();

    const std::vector<Quest>& GetQuests() const;

private:
    std::vector<Quest> quests;
    
};